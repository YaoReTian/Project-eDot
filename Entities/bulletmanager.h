#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "bullet.h"
#include "../gameobject.h"
#include "../Utils/vectorfield.h"
#include <QQueue>

class BulletManager : public GameObject
{
public:
    BulletManager(QGraphicsItem* parent = 0);
    ~BulletManager();
    void setPlayer(GameItem* p);
    virtual void update(int deltatime) override;
    void addBullet(Bullet* bullet, bool friendly = false);
    void setPrevEnemy(GameItem* enemy);
    void setParent(QGraphicsItem* parent);

    void addField(VectorField* field, QString fieldKey);
    VectorField* getField(QString fieldKey);
    Bullet* getBulletFromPool();
    void setBaseZ(qreal zValue);

    void overdrive();

    void clear();

private:
    void runOverdrive();

    bool m_overdrive;
    int m_elapsedTime;
    int m_overdriveRuns;
    QList<Bullet*> m_activeBullets;
    QList<Bullet*> m_playerBullets;
    QQueue<Bullet*> m_pool;
    QMap<QString, VectorField*> m_fields;
    QGraphicsItem* m_parent;
    qreal m_baseZ;
    TileSet* m_ts;

    GameItem* m_player;
    GameItem* m_prevEnemy;
    QPointF m_overdrivePos;
};

#endif // BULLETMANAGER_H
