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
    m_mapSizeX(0), m_mapSizeY(0), m_hitboxesVisible(false)
{
    setPen(QPen(Qt::transparent));
}

Tilemap::~Tilemap()
{
    qDeleteAll(m_sprites);
    m_sprites.clear();
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
    for (const auto o : std::as_const(objects))
    {
        QJsonObject obj = o.toObject();
        std::tie(GID,transform) = TileSet::formatGID(obj.value("gid").toInteger());
        index = TileSet::findTilesetIndex(GID, m_tilesets);
        sprite = m_tilesets[index]->getInfo(GID);
        if (!obj.value("properties").isUndefined() &&
            obj.value("properties").toArray()[0].toObject().value("value").toBool())
        {
            m_enemies.append(new Enemy(this));
            m_db->setEnemyData(sprite->m_path, m_enemies.back());
            m_enemies.back()->setTransform(transform);
            for (const auto h : std::as_const(sprite->m_hitboxes))
            {
                m_enemies.back()->setHitbox(h);
            }
            m_enemies.back()->setBulletManager(m_bulletManager);
            m_enemies.back()->update(0);
            m_enemies.back()->setPos(obj.value("x").toDouble()*GLOBAL::Scale,
                                     (obj.value("y").toDouble()-obj.value("height").toDouble())*GLOBAL::Scale);
            m_enemies.back()->setZValue(zValue);
        }
        else
        {
            m_sprites.append(new Sprite(this));
            m_db->setSpriteData(sprite->m_path, m_sprites.back());
            m_sprites.back()->setTransform(transform);
            for (const auto h : std::as_const(sprite->m_hitboxes))
            {
                m_sprites.back()->setHitbox(h);
            }
            m_sprites.back()->update(0);
            m_sprites.back()->setPos(obj.value("x").toDouble()*GLOBAL::Scale,
                                     (obj.value("y").toDouble()-obj.value("height").toDouble())*GLOBAL::Scale);
            m_sprites.back()->setZValue(zValue);
        }
    }
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

void Tilemap::input(KeyMap* keys)
{
    if (keys->keyHeldStatus(GLOBAL::SHOW_HITBOX))
    {
        m_hitboxesVisible = true;
        for (const auto &l : std::as_const(m_layers))
        {
            l->showHitboxes();
        }
        for (const auto &s: std::as_const(m_sprites))
        {
            s->showHitbox();
        }
    }
    else if (m_hitboxesVisible)
    {
        m_hitboxesVisible = false;
        for (const auto &l : std::as_const(m_layers))
        {
            l->hideHitboxes();
        }
        for (const auto &s : std::as_const(m_sprites))
        {
            s->hideHitbox();
        }
    }
}

void Tilemap::update(int deltatime)
{
    for (auto e : m_enemies)
    {
        e->update(deltatime);
    }
}
