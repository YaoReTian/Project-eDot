#include "bulletmanager.h"

#include "../tileset.h"

BulletManager::BulletManager()
{
    m_bulletFields.append(new BulletField);
}

BulletManager::~BulletManager()
{
    for (auto b : m_bulletFields)
    {
        qDeleteAll(b->m_fields);
        b->m_bullets.clear();
        b->m_fields.clear();
    }
    qDeleteAll(m_bulletFields);
    m_bulletFields.clear();
}

void BulletManager::update(int deltatime)
{
    QList<int> fieldToDelete;
    for (const auto b : std::as_const(m_bulletFields))
    {
        int i = 0;
        int size = b->m_bullets.size();
        while (i < size && !b->m_bullets.empty())
        {
            if (!b->m_fields.empty())
            {
                Vector v(0,0);
                for (auto f: std::as_const(b->m_fields))
                {
                    v += f->getVector(b->m_bullets[i]->centre());
                }
                b->m_bullets[i]->setVector(v);
            }
            b->m_bullets[i]->update(deltatime);
            if (b->m_bullets[i]->collided())
            {
                delete b->m_bullets.takeAt(i);
                i--;
                size--;
            }
            i++;
        }
        if (b->m_bullets.empty())
        {
            fieldToDelete.append(m_bulletFields.indexOf(b));
        }
    }
    int k = 0;
    for (const auto i : fieldToDelete)
    {
        delete m_bulletFields.takeAt(i-k);
        k++;
    }
}

void BulletManager::addBullet(Bullet* bullet)
{
    m_bulletFields[0]->m_bullets.append(bullet);
}

void BulletManager::addBulletField(BulletField *field)
{
    m_bulletFields.append(field);
}
