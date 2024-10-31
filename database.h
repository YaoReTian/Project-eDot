#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "Entities/sprite.h"
#include "Entities/enemy.h"
#include "Utils/global.h"

class Database
{
public:
    Database();
    ~Database();

    bool isOpen() const;

    QSqlQuery getMapInfo(int MapID);
    Sprite* getSprite(int SpriteID);
    Sprite* getSprite(QString path, QGraphicsItem* parent = 0);
    BulletManager* getBulletManager();
    void setSpriteData(QString path, Sprite* source);
    void setEnemyData(QString path, Enemy* source);
    QSqlQuery getSpriteAnimations(int SpriteID);
    QSqlQuery getSpriteTransitions(int SpriteID, QString startStateName);
    GLOBAL::Action stringToAction(QString string);

private:
    QSqlDatabase m_db;
};

#endif // DATABASE_H
