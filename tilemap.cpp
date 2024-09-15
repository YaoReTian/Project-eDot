#include "tilemap.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include <QGraphicsItemGroup>

#include "Utils/global.h"
#include "Entities/sprite.h"

// Testing includes
#include "Utils/jsonparser.h"
#include "tilelayer.h"

Tilemap::Tilemap(QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_mapID(0), m_mapName("Unset"), m_mapDesc("Unset"),
    m_mapSizeX(0), m_mapSizeY(0), m_enteredCombatIndex(0)
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

void Tilemap::setMap(int MapID, UserInterface* UI)
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
        m_sprites.append(m_db->getSprite(sprite->m_path, this));
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

void Tilemap::clear(QGraphicsScene &scene)
{
    for (const auto s : m_sprites)
    {
        s->clear(scene);
    }
}

void Tilemap::update(int deltatime)
{
    for (const auto s : m_sprites)
    {
        s->setAction(GLOBAL::NONE);
        s->update(deltatime);
    }
}

void Tilemap::render(QGraphicsScene &scene)
{
    for (const auto &l : std::as_const(m_layers))
    {
        l->render(scene);
    }
    for (const auto &s : std::as_const(m_sprites))
    {
        s->render(scene);
    }
}

void Tilemap::setSprites(UserInterface* UI)
{
    /*
    m_sprites = m_db->getWorldSprites(m_mapID);

    for (const auto s : m_sprites)
    {
        if (s->isInteractive())
        {
            UI->addPopup(s->getButton(), s->getScript());
        }
    }*/
}

void Tilemap::generateTiles(QGraphicsScene &scene)
{
    /*
    int size;
    int numTiles;
    int GID;
    int tilesetIndex;
    int chunkH;
    int chunkW;

    size = m_layers[0]->numberOfChunks();
    for (int n = 0; n < size; n++)
    {
        chunkH = m_layers[0]->getChunk(n)->size().height();
        chunkW = m_layers[0]->getChunk(n)->size().width();
        QPoint pos = m_layers[0]->getChunk(n)->pos();
        numTiles = chunkH * chunkW;

        for (int v = 0; v < numTiles; v++)
        {
            GID = m_layers[0]->getChunk(n)->tileData(v)->m_GID;
            QTransform transform = *m_layers[0]->getChunk(n)->tileData(v)->m_transform;
            tilesetIndex = getTilesetIndex(GID);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(
                QPixmap(m_tilesets[tilesetIndex]->getPixmapPathFromGID(GID)).transformed(transform));
            item->setPos(pos.x()+v%chunkW, pos.y()+v/chunkW);
            scene.addItem(item);
        }
    }
    int tileIndex = 0;

    for (int y = 0; y < GLOBAL::ObjectLength*m_mapSizeY; y += GLOBAL::ObjectLength)
    {
        for (int x = 0; x < GLOBAL::ObjectLength*m_mapSizeX; x += GLOBAL::ObjectLength)
        {
            m_tiles[tileIndex]->setPos(x,y);
            m_tiles[tileIndex]->setZValue(GLOBAL::TILE_LAYER);
            scene.addItem(m_tiles[tileIndex]);
            tileIndex++;
            if (tileIndex == m_tiles.size()) break;
        }
        if (tileIndex == m_tiles.size()) break;
    }*/
}

void Tilemap::generateSprites(QGraphicsScene &scene)
{
    for (const auto n : m_sprites)
    {
        n->setDefaultToWalk();
        n->update(0);
        scene.addItem(n);
    }
}
