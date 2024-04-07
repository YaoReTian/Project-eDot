#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "tile.h"
#include "database.h"
#include "Entities/movingsprite.h"
#include "UI/userinterface.h"

class Tilemap
{
public:
    Tilemap(Database* db);
    ~Tilemap();
    void setMap(int MapID, UserInterface* UI);
    QString getMapName();
    QString getMapDesc();
    int getMapSizeX();
    int getMapSizeY();

    void removeIem(QGraphicsScene &scene);
    void update(int deltatime, UserInterface* UI, QGraphicsItem* activeCharacter);
    void render(QGraphicsScene &scene);

    void generateTiles(QGraphicsScene &scene);
    void generateSprites(QGraphicsScene &scene);

private:
    // Methods for creating the map
    void setTiles();
    void setSprites(UserInterface* UI);

    // Attributes for creating the map
    Database* m_db;

    // Attributes of objects on the map
    QList<Sprite*> m_sprites;
    QList<MovingSprite*> m_movingSprites;
    QList<Tile*> m_tiles;

    // Map info
    int m_mapID;
    QString m_mapName;
    QString m_mapDesc;
    int m_mapSizeX;
    int m_mapSizeY;
};

#endif // MAP_H
