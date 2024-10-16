#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include "../gameobject.h"
#include "../utils/vector.h"

class Bullet : public QGraphicsPixmapItem, public GameObject
{
public:
    Bullet(QGraphicsItem* parent = 0);
    ~Bullet();
    void setFriendly(bool value = true);
    virtual void update(int deltaTime) override;

    void setVector(Vector v);
    void setVector(qreal i, qreal j);
    void setUnitSpeed(qreal spd);

    Vector vector() const;
    qreal i() const;
    qreal j() const;
    bool collided() const;
    bool isFriendly() const;
    QPointF centre() const;
    qreal unitSpeed() const;

private:
    Vector m_vector;
    int m_dmg;
    bool m_friendly;
    qreal m_unitSpeed; // scale factor for a unit vector
};

#endif // BULLET_H
