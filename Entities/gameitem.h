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

    QPointF centre() const;
    Vector vector() const;
    qreal i() const;
    qreal j() const;
    qreal angle() const;

private:
    Vector m_vector;
    qreal m_baseZ;
};

#endif // GAMEITEM_H
