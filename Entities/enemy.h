#ifndef ENEMY_H
#define ENEMY_H

#include "sprite.h"
#include "bulletmanager.h"

/*
 * What we need:
 *
 *  Pattern:
 *      Number of bullets
 *      Trajectory (field)
 *      Bullet spawn rate
 *      Type of bullet
 *
 *  Phase:
 *      List of patterns
 *      Phase time
 *      HP
 */

struct Pattern
{
    QList<Bullet*> m_bullets;
    int spawnRate;
};

struct Phase
{
    QList<Pattern*> m_patterns;
    int m_phaseTime;
    int m_hp;
};

class Enemy : public Sprite
{
public:
    Enemy(QGraphicsItem* parent = 0);
    ~Enemy();

    virtual void update(int deltaTime) override;
    void setBulletManager(BulletManager* manager);
    void setFieldKey(QString key);
    void addPhase(Phase* phase);

private:
    BulletManager* m_bulletManager;
    QList<QString> m_fieldKeys;
    QQueue<Phase*> m_phases;
};

#endif // ENEMY_H
