#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "bullet.h"
#include "../gameobject.h"
#include "../Utils/vectorfield.h"

struct BulletField
{
    QList<VectorField*> m_fields;
    QList<Bullet*> m_bullets;
};

class BulletManager : public GameObject
{
public:
    BulletManager();
    ~BulletManager();
    virtual void update(int deltatime) override;
    void addBullet(Bullet* bullet);
    void addBulletField(BulletField* field);
    void addBulletToField(Bullet* bullet);

private:
    QList<BulletField*> m_bulletFields;
};

#endif // BULLETMANAGER_H
