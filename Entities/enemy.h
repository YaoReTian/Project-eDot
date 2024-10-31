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
    QList<QPointF> m_spawnPos;
    QString m_fieldKey;
    int m_spawnRate;
    int m_elapsedTime;
};

struct Phase
{
    QList<Pattern*> m_patterns;
    int m_phaseTime;
    int m_hp;

    ~Phase()
    {
        qDeleteAll(m_patterns);
        m_patterns.clear();
    }
};

class Enemy : public Sprite
{
public:
    Enemy(QGraphicsItem* parent = 0);
    ~Enemy();

    void input(QPointF playerPos);
    virtual void update(int deltaTime) override;
    void setBulletManager(BulletManager* manager);
    void addFieldKey(QString key, QPointF origin);
    void removeFieldKey(QString key);
    void addPhase(Phase* phase);
    void takeDmg(int dmg);

    int HP() const;

    void hide();

private:

    void updatePhases(int deltatime);

    BulletManager* m_bulletManager;
    QList<QString> m_fieldKeys;
    QList<QPointF> m_fieldOrigins;
    QList<Phase*> m_phases;
    int m_HP;
    int m_phaseIndex;
    int m_elapsedTime;
    bool m_playerDetected;
};

#endif // ENEMY_H
