#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "tile.h"
#include "Entities/interactivesprite.h"
#include "Utils/global.h"

class Database
{
public:
    Database();
    ~Database();

    bool isOpen() const;

    QString getTestTilePath();

    QSqlQuery getMapInfo(int MapID);
    QList<Tile*> getMapTiles(int MapID);
    QList<InteractiveSprite*> getWorldSprites(int MapID);
    Sprite* getSprite(int SpriteID);
    QSqlQuery getSpriteAnimations(int SpriteID);
    QSqlQuery getSpriteTransitions(int AnimationID);
    GLOBAL::Action stringToAction(QString string);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
