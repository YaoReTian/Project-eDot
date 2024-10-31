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
    virtual void update(int deltatime) override;
    void addBullet(Bullet* bullet);
    void setParent(QGraphicsItem* parent);

    void addField(VectorField* field, QString fieldKey);
    VectorField* getField(QString fieldKey);
    Bullet* getBulletFromPool();

    void clear();

private:
    QList<Bullet*> m_activeBullets;
    QQueue<Bullet*> m_pool;
    QMap<QString, VectorField*> m_fields;
    QGraphicsItem* m_parent;
};

#endif // BULLETMANAGER_H
