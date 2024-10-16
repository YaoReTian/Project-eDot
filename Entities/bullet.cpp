#include "bullet.h"
#include "../Utils/global.h"

#include "../tileset.h"

Bullet::Bullet(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), m_vector(0,0), m_dmg(1), m_friendly(false)
{
    TileSet t(":/tileset/res/Basic bullets.tsj",1);
    setPixmap(t.getInfo(182)->m_pixmap);

}

Bullet::~Bullet()
{

}

void Bullet::update(int deltaTime)
{
    setPos(x() + m_unitSpeed * m_vector.i() * GLOBAL::ObjectLength * deltaTime,
           y() + m_unitSpeed * m_vector.j() * GLOBAL::ObjectLength * deltaTime);
    setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);
    setRotation(m_vector.angle());
}

void Bullet::setUnitSpeed(qreal spd)
{
    m_unitSpeed = spd;
}

void Bullet::setVector(Vector v)
{
    m_vector = v;
}

void Bullet::setVector(qreal i, qreal j)
{
    m_vector.setVector(i,j);
}

Vector Bullet::vector() const
{
    return m_vector;
}

qreal Bullet::i() const
{
    return m_vector.i();
}

qreal Bullet::j() const
{
    return m_vector.j();
}

bool Bullet::collided() const
{
    QList<QGraphicsItem*> list = collidingItems();
    for (const auto c : list)
    {
        if (typeid(*c) == typeid(Hitbox) &&
            dynamic_cast<Hitbox*>(c)->m_type == Wall)
        {
            return true;
        }
    }
    return false;
}

bool Bullet::isFriendly() const
{
    return m_friendly;
}

QPointF Bullet::centre() const
{
    return QPointF(x() + boundingRect().width()/2, y() + boundingRect().height()/2);
}

qreal Bullet::unitSpeed() const
{
    return m_unitSpeed;
}
