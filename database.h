#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlQuery>

#include "Entities/sprite.h"
#include "Entities/enemy.h"
#include "Utils/global.h"

struct SaveSlot
{
    QString m_username;
    qreal m_tilePosX;
    qreal m_tilePosY;
    int m_mapID;
};

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

    void saveGame(int slotID, QString username, qreal tilePosX, qreal tilePosY, int mapID);
    SaveSlot* getSaveSlot(int slotID);

private:
    SaveSlot* m_saveSlots[5];
    QSqlDatabase m_db;
};

#endif // DATABASE_H
