#include "enemy.h"

Enemy::Enemy(QGraphicsItem* parent)
    : Sprite(parent), m_phaseIndex(-1), m_elapsedTime(0)
{

}

Enemy::~Enemy()
{
    qDeleteAll(m_phases);
    m_phases.clear();
}

void Enemy::update(int deltaTime)
{
    if (m_phaseIndex == -1)
    {
        m_phaseIndex++;
        m_HP = m_phases[0]->m_hp;
    }
    if (m_phaseIndex < m_phases.size())
    {
        m_elapsedTime += deltaTime;
        if (m_phases[m_phaseIndex]->m_phaseTime != -1 &&
            (m_elapsedTime >= m_phases[m_phaseIndex]->m_phaseTime || m_HP == 0))
        {
            m_phaseIndex++;
            m_HP = m_phases[m_phaseIndex]->m_hp;
            m_elapsedTime = 0;
        }
        else
        {
            updatePhases(deltaTime);
        }
    }
    Sprite::update(deltaTime);
}

void Enemy::updatePhases(int deltatime)
{
    for (auto p : m_phases[m_phaseIndex]->m_patterns)
    {
        p->m_elapsedTime += deltatime;
        if (p->m_elapsedTime >= p->m_spawnRate)
        {
            p->m_elapsedTime = 0;
            for (auto v : p->m_spawnPos)
            {
                Bullet* b = m_bulletManager->getBulletFromPool();
                b->setPos(centre().x() + v.x(), centre().y() + v.y());
                b->setUnitSpeed(4.5f/1000.0f);
                VectorField* f = m_bulletManager->getField(p->m_fieldKey);
                b->show();
                b->addField(f,centre().x(), centre().y());
                m_bulletManager->addBullet(b);
            }
        }
    }
}

void Enemy::setBulletManager(BulletManager* manager)
{
    m_bulletManager = manager;
}

void Enemy::addFieldKey(QString key)
{
    m_fieldKeys.append(key);
}

void Enemy::removeFieldKey(QString key)
{
    m_fieldKeys.removeOne(key);
}

void Enemy::addPhase(Phase* phase)
{
    m_phases.append(phase);
}

int Enemy::HP() const
{
    return m_HP;
}
