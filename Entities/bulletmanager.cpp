#include "bulletmanager.h"

#include "../tileset.h"

BulletManager::BulletManager(QGraphicsItem* parent)
    : m_parent(parent)
{
    m_fields["UniformPositiveRadial"] = new VectorField("x/sqrt(x^2 + y^2)", "y/sqrt(x^2 + y^2)");
    m_fields["UniformNegativeRadial"] = new VectorField("-x/sqrt(x^2 + y^2)", "-y/sqrt(x^2 + y^2)");

    for (int i = 0; i < 1000; i++)
    {
        m_pool.append(new Bullet);
    }
}

BulletManager::~BulletManager()
{
    m_activeBullets.clear();
    m_pool.clear();
    qDeleteAll(m_fields);
    m_fields.clear();
}

void BulletManager::setParent(QGraphicsItem *parent)
{
    m_parent = parent;
}

void BulletManager::update(int deltatime)
{
    int i = 0;
    int size = m_activeBullets.size();
    while (i < size && !m_activeBullets.empty())
    {
        m_activeBullets[i]->update(deltatime);
        if (m_activeBullets[i]->collided())
        {
            m_activeBullets[i]->hide();
            m_pool.enqueue(m_activeBullets.takeAt(i));
            i--;
            size--;
        }
        i++;
    }
}

void BulletManager::addBullet(Bullet* bullet)
{
    m_activeBullets.append(bullet);
}

Bullet* BulletManager::getBulletFromPool()
{
    if (m_pool.empty())
    {
        for (int i = 0; i < 10; i++)
        {
            m_pool.append(new Bullet);
        }
    }
    Bullet* b = m_pool.dequeue();
    b->setParentItem(m_parent);
    b->setZValue(m_baseZ);
    return b;
}

void BulletManager::addField(VectorField* field, QString fieldKey)
{
    m_fields[fieldKey] = field;
}

VectorField* BulletManager::getField(QString fieldKey)
{
    if (!m_fields.contains(fieldKey))
    {
        qDebug() << "Error, field key not found";
        return nullptr;
    }
    return m_fields[fieldKey];
}

void BulletManager::clear()
{
    while (!m_activeBullets.empty())
    {
        m_activeBullets.back()->hide();
        m_pool.enqueue(m_activeBullets.takeLast());
    }
}

void BulletManager::setBaseZ(qreal zValue)
{
    m_baseZ = zValue;
}
