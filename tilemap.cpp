#include "tilemap.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QGraphicsItemGroup>

#include "Utils/global.h"
#include "Utils/jsonparser.h"
#include "tilelayer.h"
#include "Entities/sprite.h"

Tilemap::Tilemap(QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_mapID(0), m_mapName("Unset"), m_mapDesc("Unset"),
    m_mapSizeX(0), m_mapSizeY(0), m_startPos(10,10), m_hitboxesVisible(false)
{
    setPen(QPen(Qt::transparent));
}

Tilemap::~Tilemap()
{
    clear();
}

void Tilemap::setDatabase(Database* db)
{
    m_db = db;
}

void Tilemap::setBulletManager(BulletManager *manager)
{
    m_bulletManager = manager;
    m_bulletManager->setParent(this);
}

void Tilemap::setMap(int MapID)
{
    clear();
    QSqlQuery query;

    if (m_db->isOpen()) {
        query = m_db->getMapInfo(MapID);
    } else {
        qDebug() << "Database is not open";
    }

    query.next();
    m_mapID = query.value("MapID").toInt();
    m_mapName = query.value("MapName").toString();
    m_mapDesc = query.value("MapDescription").toString();
    m_startPos.setX(query.value("StartX").toInt()*GLOBAL::ObjectLength);
    m_startPos.setY(query.value("StartY").toInt()*GLOBAL::ObjectLength);

    QJsonObject mapData = JsonParser::parse(query.value("MapPath").toString());
    m_mapSizeX = mapData.value("width").toInt();
    m_mapSizeY = mapData.value("height").toInt();
    setRect(0,0,m_mapSizeX*GLOBAL::ObjectLength, m_mapSizeY*GLOBAL::ObjectLength);
    m_infinite = mapData.value("infinite").toBool();
    m_backgroundColour = mapData.value("backgroundcolor").toInt();

    QJsonArray sets = mapData.value("tilesets").toArray();

    for (const auto &set : std::as_const(sets))
    {
        QJsonObject s = set.toObject();
        m_tilesets.append(new TileSet(JsonParser::parsePath(s.value("source").toString()),
                                      s.value("firstgid").toInt()));
    }

    QJsonArray layers = mapData.value("layers").toArray();
    int numSpriteLayers = 0;
    for (int i = 0; i < layers.size(); i++)
    {
        QJsonObject l = layers[i].toObject();
        if (l.value("type").toString() == "tilelayer")
        {
            createTileLayer(l, i + numSpriteLayers*m_mapSizeY*GLOBAL::ObjectLength);
        }
        else if (l.value("type").toString() == "objectgroup")
        {
            createSpriteLayer(l, i + numSpriteLayers*m_mapSizeY*GLOBAL::ObjectLength);
            numSpriteLayers++;
        }
    }
    m_bulletManager->setBaseZ(layers.size() + numSpriteLayers*m_mapSizeY*GLOBAL::ObjectLength);
}

void Tilemap::setPlayerStartPos(QPointF pos)
{
    m_startPos = pos;
}

void Tilemap::createTileLayer(QJsonObject chunks, int zValue)
{
    m_layers.append(new TileLayer(this));
    m_layers.back()->setBgColour(Qt::transparent);
    m_layers.back()->setZValue(zValue);
    m_layers.back()->setChunks(chunks.value("chunks").toArray(),m_tilesets);
}

void Tilemap::createSpriteLayer(QJsonObject objectGroup, int zValue)
{
    if (objectGroup.value("name").toString() == "interactable")
    {
        m_playerZ = zValue;
    }
    int index;
    TileInfo* sprite;
    int GID;
    QTransform transform;
    QJsonArray objects = objectGroup.value("objects").toArray();
    QJsonObject s;
    int mapID = -1;

    // Creating sprites and enemies
    for (auto o : objects)
    {
        mapID = -1;
        s = o.toObject();
        std::tie(GID,transform) = TileSet::formatGID(s.value("gid").toInteger());
        index = TileSet::findTilesetIndex(GID, m_tilesets);
        sprite = m_tilesets[index]->getInfo(GID);
        if (!s.value("properties").isUndefined() &&
            s.value("properties").toArray()[0].toObject().value("name").toString() == "Enemy")
        {
            m_enemies.append(new Enemy(this));
            m_db->setEnemyData(sprite->m_path, m_enemies.back());
            m_enemies.back()->setTransform(transform);
            for (auto h : sprite->m_hitboxes)
            {
                m_enemies.back()->setHitbox(h);
            }
            m_enemies.back()->setBulletManager(m_bulletManager);
            m_enemies.back()->update(0);
            m_enemies.back()->setPos(s.value("x").toDouble()*GLOBAL::Scale,
                                     (s.value("y").toDouble()-s.value("height").toDouble())*GLOBAL::Scale);
            m_enemies.back()->setZValue(zValue);
        }
        else
        {
            m_sprites.append(new Sprite(this));
            m_db->setSpriteData(sprite->m_path, m_sprites.back());
            m_sprites.back()->setTransform(transform);
            if (!s.value("properties").isUndefined() &&
                s.value("properties").toArray()[0].toObject().value("name").toString() == "MapLink")
            {
                mapID = s.value("properties").toArray()[0].toObject().value("value").toInt();
            }
            for (auto h : sprite->m_hitboxes)
            {
                h->m_mapLinkID = mapID;
                m_sprites.back()->setHitbox(h);
            }
            m_sprites.back()->update(0);
            m_sprites.back()->setPos(s.value("x").toDouble()*GLOBAL::Scale,
                                     (s.value("y").toDouble()-s.value("height").toDouble())*GLOBAL::Scale);
            m_sprites.back()->setZValue(zValue);
        }
    }
}

int Tilemap::mapID()
{
    return m_mapID;
}

QString Tilemap::getMapDesc()
{
    return m_mapDesc;
}

QString Tilemap::getMapName()
{
    return m_mapName;
}

int Tilemap::getMapSizeX()
{
    return m_mapSizeX;
}

int Tilemap::getMapSizeY()
{
    return m_mapSizeY;
}

qreal Tilemap::getPlayerZ()
{
    return m_playerZ;
}

QRgb Tilemap::bgColour()
{
    return m_backgroundColour;
}

void Tilemap::input(KeyMap* keys, QPointF playerPos)
{
    if (keys->keyHeldStatus(GLOBAL::SHOW_HITBOX))
    {
        m_hitboxesVisible = true;
        for (auto &l : m_layers)
        {
            l->showHitboxes();
        }
        for (auto &s: m_sprites)
        {
            s->showHitbox();
        }
        for (auto &e : m_enemies)
        {
            e->showHitbox();
        }
    }
    else if (m_hitboxesVisible)
    {
        m_hitboxesVisible = false;
        for (auto &l : m_layers)
        {
            l->hideHitboxes();
        }
        for (auto &s : m_sprites)
        {
            s->hideHitbox();
        }
        for (auto &e : m_enemies)
        {
            e->hideHitbox();
        }
    }
    for (auto &e : m_enemies)
    {
        e->input(playerPos);
    }
}

void Tilemap::update(int deltatime)
{
    int i = 0;
    int size = m_enemies.size();
    while (i < size && !m_enemies.empty())
    {
        m_enemies[i]->update(deltatime);
        if (m_enemies[i]->dead())
        {
            m_enemies[i]->hide();
            m_enemyPool.enqueue(m_enemies.takeAt(i));
            i--;
            size--;
        }
        i++;
    }
}

QPointF Tilemap::playerStartPos()
{
    return m_startPos;
}

bool Tilemap::enemiesCleared()
{
    if (m_enemies.size() == 0)
    {
        return true;
    }
    return false;
}

void Tilemap::reset()
{
    while (!m_enemyPool.empty())
    {
        m_enemies.append(m_enemyPool.dequeue());
    }
    m_bulletManager->clear();
}

void Tilemap::clear()
{
    qDeleteAll(m_sprites);
    m_sprites.clear();
    qDeleteAll(m_sprites);
    m_sprites.clear();
    qDeleteAll(m_enemies);
    m_enemies.clear();
    qDeleteAll(m_enemyPool);
    m_enemyPool.clear();
    qDeleteAll(m_layers);
    m_layers.clear();
    qDeleteAll(m_tilesets);
    m_tilesets.clear();
}
