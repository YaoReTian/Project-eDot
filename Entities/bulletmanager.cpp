#include "bulletmanager.h"

#include "../tileset.h"

BulletManager::BulletManager() {}

BulletManager::~BulletManager()
{
    qDeleteAll(m_bullets);
    m_bullets.clear();
}

void BulletManager::update(int deltatime)
{
    int i = 0;
    int size = m_bullets.size();

    while (i < size && !m_bullets.empty())
    {
        m_bullets[i]->update(deltatime);
        if (bulletRemoved(m_bullets[i]))
        {
            delete m_bullets.takeAt(i);
            i--;
            size--;
        }
        i++;
    }
}

bool BulletManager::bulletRemoved(Bullet *bullet)
{
    QList<QGraphicsItem*> list = bullet->collidingItems();
    for (const auto c : list)
    {
        if (typeid(*c) == typeid(Hitbox) &&
            dynamic_cast<Hitbox*>(c)->m_type == Wall)
        {
            return true;
        }
    }
    return false;
}

void BulletManager::addBullet(Bullet *bullet)
{
    m_bullets.append(bullet);
}
