#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "bullet.h"
#include "../gameobject.h"
#include "../utils/vectorfield.h"

class BulletManager : public GameObject
{
public:
    BulletManager();
    ~BulletManager();
    void update(int deltatime);
    void addBullet(Bullet* bullet);

private:
    bool bulletRemoved(Bullet* bullet);
    QList<Bullet*> m_bullets;
};

#endif // BULLETMANAGER_H
