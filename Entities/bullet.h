#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include "../gameobject.h"
#include "../utils/vector.h"

class Bullet : public QGraphicsPixmapItem, public GameObject, public Vector
{
public:
    Bullet(QGraphicsItem* parent = 0);
    ~Bullet();
    void setFriendly(bool value = true);
    virtual void update(int deltaTime) override;

    bool isFriendly();
private:
    int m_dmg;
    bool m_friendly;
};

#endif // BULLET_H
