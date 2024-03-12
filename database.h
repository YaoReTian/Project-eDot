#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "sprite.h"
#include "tile.h"
#include "movingsprite.h"

class Database
{
public:
    Database();
    ~Database();

    bool isOpen() const;

    QString getTestTilePath();

    QSqlQuery getMapInfo(int MapID);
    QList<Tile*> getMapTiles(int MapID);
    QList<Sprite*> getWorldSprites(int MapID);
    QList<MovingSprite*> getMovingSpritesFromMap(int MapID);
    MovingSprite* getMovingSprite(int SpriteID);
    QSqlQuery getSpriteAnimations(int SpriteID);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
