#include "tilemap.h"

#include <QDebug>
#include <QSqlQuery>
#include <QPixmap>

#include "global.h"

// Public methods
Tilemap::Tilemap(int MapID, QGraphicsScene &scene)
{
    QSqlQuery query;

    if (m_db.isOpen()) {
        query = m_db.getMapInfo(MapID);
    } else {
        qDebug() << "Database is not open";
    }

    query.next();
    m_mapID = query.value("MapID").toInt();
    m_mapName = query.value("MapName").toString();
    m_mapDesc = query.value("MapDescription").toString();
    m_mapSizeX = query.value("MapSizeX").toInt();
    m_mapSizeY = query.value("MapSizeY").toInt();

    generateTiles(scene);
}

QString Tilemap::getMapDesc()
{
    return m_mapDesc;
}

QString Tilemap::getMapName()
{
    return m_mapName;
}

void Tilemap::update()
{

}

// Private methods
void Tilemap::generateTiles(QGraphicsScene &scene)
{
    QSqlQuery query;
    QString filePath;
    QPixmap image;
    int NumberOfTiles;

    if (m_db.isOpen())
    {
        query = m_db.getMapTiles(m_mapID);
    }
    else
    {
        qDebug() << "Database is not open";
    }

    while (query.next())
    {
        filePath = query.value("PathToTileSheet").toString();
        NumberOfTiles = query.value("NumberOfTiles").toInt();
        image.load(filePath);

        qDebug() << image.size();

        for (int n = 0; n < NumberOfTiles; n++)
        {
            m_tiles.append(new Tile);
            m_tiles.back()->setPixmap(image.scaled(GLOBAL::ObjectSize));
            m_tiles.back()->setID(query.value("Tile.TileID").toInt());
            m_tiles.back()->setDescription(query.value("TileDescription").toString());
            m_tiles.back()->setName(query.value("TileName").toString());

            qDebug() << m_tiles.back()->getID();
            qDebug() << m_tiles.back()->getName();
            qDebug() << m_tiles.back()->getDescription();
        }
    }

    if (m_mapSizeX * m_mapSizeY != m_tiles.size()) {
        qDebug() << m_mapSizeX * m_mapSizeY;
        qDebug() << m_tiles.size();
        qDebug() << "ERROR: NUMBER OF TILES != AREA";
    }

    int tileIndex = 0;

    for (int y = 0; y < GLOBAL::ObjectLength*m_mapSizeY; y += GLOBAL::ObjectLength)
    {
        for (int x = 0; x < GLOBAL::ObjectLength*m_mapSizeX; x += GLOBAL::ObjectLength)
        {
            qDebug() << "Position:" << x << y;
            m_tiles[tileIndex]->setPos(x,y);
            scene.addItem(m_tiles[tileIndex]);
            tileIndex++;
        }
    }
}

void Tilemap::generateSprites(QGraphicsScene &scene)
{

}

void Tilemap::updateTiles()
{

}

void Tilemap::updateSprites()
{

}
