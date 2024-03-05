#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "tile.h"

#include "database.h"
class Tilemap
{
public:
    Tilemap(int MapID, QGraphicsScene &scene);
    QString getMapName();
    QString getMapDesc();

    void update();

private:
    // Methods for creating the map
    void generateTiles(QGraphicsScene &scene);
    void generateSprites(QGraphicsScene &scene);

    // Methods for updating the map
    void updateSprites();
    void updateTiles();

    // Attributes for creating the map
    Database m_db;

    // Attributes of objects on the map
    QList<QGraphicsPixmapItem> m_sprites;
    QList<Tile*> m_tiles;

    // Map info
    int m_mapID;
    QString m_mapName;
    QString m_mapDesc;
    int m_mapSizeX;
    int m_mapSizeY;
};

#endif // MAP_H
