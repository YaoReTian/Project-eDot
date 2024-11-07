#include "bulletmanager.h"

#include "../tileset.h"
#include "../Utils/global.h"

BulletManager::BulletManager(QGraphicsItem* parent)
    : m_overdrive(false), m_elapsedTime(0), m_overdriveRuns(0), m_parent(parent),
    m_ts(new TileSet(":/tileset/res/Basic bullets.tsj",1)), m_prevEnemy(nullptr)
{
    for (int i = 0; i < 1000000; i++)
    {
        m_pool.append(new Bullet(m_ts));
    }
}

BulletManager::~BulletManager()
{
    m_activeBullets.clear();
    m_pool.clear();
    qDeleteAll(m_fields);
    m_fields.clear();
    delete m_ts;
}

void BulletManager::setPlayer(GameItem* p)
{
    m_player = p;
}

void BulletManager::setPrevEnemy(GameItem *enemy)
{
    m_prevEnemy = enemy;
}

void BulletManager::setParent(QGraphicsItem *parent)
{
    m_parent = parent;
}

void BulletManager::update(int deltatime)
{
    if (m_overdrive)
    {
        m_elapsedTime += deltatime;
        if (m_elapsedTime >= 60)
        {
            m_overdriveRuns++;
            m_elapsedTime = 0;
            runOverdrive();
        }
        if (m_overdriveRuns == 15)
        {
            m_overdrive = false;
            m_overdriveRuns = 0;
            m_elapsedTime = 0;
        }
    }

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

    i = 0;
    size = m_playerBullets.size();
    while (i < size && !m_playerBullets.empty())
    {
        m_playerBullets[i]->update(deltatime);
        if (m_playerBullets[i]->collided())
        {
            m_playerBullets[i]->hide();
            m_pool.enqueue(m_playerBullets.takeAt(i));
            i--;
            size--;
        }
        i++;
    }
}

void BulletManager::addBullet(Bullet* bullet, bool friendly)
{
    if (friendly)
    {
        m_playerBullets.append(bullet);
    }
    else
    {
        m_activeBullets.append(bullet);
    }
}

Bullet* BulletManager::getBulletFromPool()
{
    if (m_pool.empty())
    {
        for (int i = 0; i < 100; i++)
        {
            m_pool.enqueue(new Bullet(m_ts));
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
    while (!m_playerBullets.empty())
    {
        m_playerBullets.back()->hide();
        m_pool.enqueue(m_playerBullets.takeLast());
    }
}

void BulletManager::setBaseZ(qreal zValue)
{
    m_baseZ = zValue;
}

void BulletManager::overdrive()
{
    m_overdrive = true;
    m_overdrivePos = m_player->centre();
}

void BulletManager::runOverdrive()
{
    int t = GLOBAL::ObjectLength;
    int t2 = 2*t;
    QList<QPointF> bulletPos = {QPointF(-1,-1), QPointF(-1,1), QPointF(-1,0), QPointF(0,-1),
                                QPointF(0,1), QPointF(1,0), QPointF(1,-1), QPointF(1,1),
                                QPointF(-t,-t), QPointF(-t,t), QPointF(-t,0), QPointF(0,-t),
                                QPointF(0,-t), QPointF(t,0), QPointF(t,-t), QPointF(t,t),
                                QPointF(-t2,-t2), QPointF(-t2,t2), QPointF(-t2,0), QPointF(0,-t2),
                                QPointF(0,-t2), QPointF(t2,0), QPointF(t2,-t2), QPointF(t2,t2)};

    for (int i = 0; i < 24; i++)
    {
        Bullet* b = getBulletFromPool();
        b->setBulletID(166);
        b->addField(getField("UniformPositiveRadial"), m_overdrivePos);
        if (m_prevEnemy != nullptr && m_prevEnemy->engaged())
        {
            b->addField(getField("UniformNegativeRadial"), m_prevEnemy);
        }
        b->setDmg(10);
        b->show();
        b->setFriendly();
        b->setPos(bulletPos[i] + m_overdrivePos);
        addBullet(b, true);
    }
}
