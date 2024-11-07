#include "gameitem.h"

GameItem::GameItem(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent), GameObject()
{

}

void GameItem::update(int deltatime)
{
    QGraphicsPixmapItem::setZValue(m_baseZ + y() + boundingRect().height());
}

void GameItem::setVector(Vector vector)
{
    m_vector = vector;
}

void GameItem::setVector(qreal i, qreal j)
{
    m_vector.setVector(i,j);
}

void GameItem::setI(qreal i)
{
    m_vector.setI(i);
}

void GameItem::setJ(qreal j)
{
    m_vector.setJ(j);
}

void GameItem::setZValue(qreal zValue)
{
    m_baseZ = zValue;
    QGraphicsPixmapItem::setZValue(zValue + y() + boundingRect().height());
}

void GameItem::setEngaged(bool value)
{
    m_engaged = value;
}

QPointF GameItem::centre() const
{
    return QPointF(x() + boundingRect().width()/2, y()+boundingRect().height()/2);
}

Vector GameItem::vector() const
{
    return m_vector;
}

qreal GameItem::i() const
{
    return m_vector.i();
}

qreal GameItem::j() const
{
    return m_vector.j();
}

qreal GameItem::angle() const
{
    return m_vector.angle();
}

bool GameItem::engaged() const
{
    return m_engaged;
}
