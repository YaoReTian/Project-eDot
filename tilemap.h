#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "tile.h"
#include "database.h"
#include "sprite.h"

class Tilemap
{
public:
    Tilemap();
    ~Tilemap();
    void setMap(int MapID);
    QString getMapName();
    QString getMapDesc();

    void update(int deltatime);
    void generateTiles(QGraphicsScene &scene);
    void generateSprites(QGraphicsScene &scene);

private:
    // Methods for creating the map
    void setTiles();
    void setSprites();

    // Methods for updating the map
    void updateSprites();
    void updateTiles();

    // Attributes for creating the map
    Database m_db;

    // Attributes of objects on the map
    QList<Sprite*> m_sprites;
    QList<Tile*> m_tiles;

    // Map info
    int m_mapID;
    QString m_mapName;
    QString m_mapDesc;
    int m_mapSizeX;
    int m_mapSizeY;
};

#endif // MAP_H
