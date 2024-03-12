#include "tilemap.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

#include "global.h"
#include "movingsprite.h"

// Public methods
Tilemap::Tilemap(Database* db)
{
    m_db = db;
}

Tilemap::~Tilemap()
{
    qDebug() << "Tilemap destroyed";
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
    m_mapSizeX = query.value("MapSizeX").toInt();
    m_mapSizeY = query.value("MapSizeY").toInt();

    setTiles();
    setSprites();
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

void Tilemap::update(int deltatime)
{
    for (const auto &s : m_sprites)
    {
        s->update(deltatime);
    }
    for (const auto &s : m_movingSprites)
    {
        s->move(deltatime, GLOBAL::MOVE_LEFT);
        s->update(deltatime);
    }
}

void Tilemap::setTiles()
{

    m_tiles = m_db->getMapTiles(m_mapID);

    if (m_mapSizeX * m_mapSizeY != m_tiles.size()) {
        qDebug() << m_mapSizeX * m_mapSizeY;
        qDebug() << m_tiles.size();
        qDebug() << "ERROR: NUMBER OF TILES != AREA";
    }
}

void Tilemap::setSprites()
{
    m_sprites = m_db->getWorldSprites(m_mapID);
    m_movingSprites = m_db->getMovingSpritesFromMap(m_mapID);
}

void Tilemap::generateTiles(QGraphicsScene &scene)
{
    int tileIndex = 0;

    for (int y = 0; y < GLOBAL::ObjectLength*m_mapSizeY; y += GLOBAL::ObjectLength)
    {
        for (int x = 0; x < GLOBAL::ObjectLength*m_mapSizeX; x += GLOBAL::ObjectLength)
        {
            m_tiles[tileIndex]->setPos(x,y);
            m_tiles[tileIndex]->setZValue(GLOBAL::TILE_LAYER);
            scene.addItem(m_tiles[tileIndex]);
            tileIndex++;
        }
    }
}

void Tilemap::generateSprites(QGraphicsScene &scene)
{
    for (const auto &n : m_sprites)
    {
        n->update(0);
        scene.addItem(n);
    }
    for (const auto &n : m_movingSprites)
    {
        n->update(0);
        scene.addItem(n);
    }
}

void Tilemap::updateTiles()
{

}

void Tilemap::updateSprites()
{

}
