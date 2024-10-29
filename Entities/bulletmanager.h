#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "bullet.h"
#include "../gameobject.h"
#include "../Utils/vectorfield.h"
#include <QQueue>

class BulletManager : public GameObject
{
public:
    BulletManager();
    ~BulletManager();
    virtual void update(int deltatime) override;
    void addBullet(Bullet* bullet);
    void createBullet(QGraphicsItem* parent = 0);

    // Testing
    void addField(VectorField* field, QString fieldKey);
    VectorField* getField(QString fieldKey);
    void removeField(QString fieldKey);
    Bullet* getBulletFromPool();

private:
    QList<Bullet*> m_activeBullets;
    QQueue<Bullet*> m_pool;
    QMap<QString, VectorField*> m_fields;
    QMap<QString, int> m_fieldCount;
};

#endif // BULLETMANAGER_H
