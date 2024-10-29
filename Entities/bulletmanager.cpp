#include "bulletmanager.h"

#include "../tileset.h"

BulletManager::BulletManager()
{
    m_fields["UniformPositiveRadial"] = new VectorField("x/sqrt(x^2 + y^2)", "y/sqrt(x^2 + y^2)");
    m_fields["UniformNegativeRadial"] = new VectorField("-x/sqrt(x^2 + y^2)", "-y/sqrt(x^2 + y^2)");
    m_fieldCount["UniformPositiveRadial"] = 1;
    m_fieldCount["UniformNegativeRadial"] = 1;
}

BulletManager::~BulletManager()
{
    m_activeBullets.clear();
    m_pool.clear();
    qDeleteAll(m_fields);
    m_fields.clear();
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

void BulletManager::createBullet(QGraphicsItem *parent)
{
    m_pool.enqueue(new Bullet(parent));
}

Bullet* BulletManager::getBulletFromPool()
{
    if (m_pool.empty())
    {
        qDebug() << "ERROR: empty pool";
        return nullptr;
    }
    return m_pool.dequeue();
}

void BulletManager::addField(VectorField* field, QString fieldKey)
{
    if (m_fieldCount.contains(fieldKey))
    {
        m_fieldCount[fieldKey]++;
    }
    else
    {
        m_fieldCount[fieldKey] = 1;
        m_fields[fieldKey] = field;
    }
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

void BulletManager::removeField(QString fieldKey)
{
    if (!m_fields.contains(fieldKey))
    {
        qDebug() << "Error, key not found";
    }
    if (m_fieldCount[fieldKey] == 1)
    {
        delete m_fields[fieldKey];
        m_fieldCount.remove(fieldKey);
    }
    else
    {
        m_fieldCount[fieldKey]--;
    }
}
