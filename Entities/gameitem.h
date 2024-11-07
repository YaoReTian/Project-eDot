#ifndef GAMEITEM_H
#define GAMEITEM_H

#include "../gameobject.h"
#include "../Utils/vector.h"
#include <QGraphicsPixmapItem>

class GameItem : public QGraphicsPixmapItem, public GameObject
{
public:
    GameItem(QGraphicsItem* parent = 0);

    virtual void update(int deltatime) override;

    void setVector(Vector vector);
    void setVector(qreal i, qreal j);
    void setI(qreal i);
    void setJ(qreal j);
    void setZValue(qreal zValue);
    void setEngaged(bool value);

    QPointF centre() const;
    Vector vector() const;
    qreal i() const;
    qreal j() const;
    qreal angle() const;
    bool engaged() const;

private:
    Vector m_vector;
    qreal m_baseZ;
    bool m_engaged;
};

#endif // GAMEITEM_H
