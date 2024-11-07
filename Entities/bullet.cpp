#include "bullet.h"
#include "../Utils/global.h"

#include "enemy.h"
#include "player.h"

Bullet::Bullet(TileSet* ts, QGraphicsItem* parent)
    : GameItem(parent), m_dmg(1), m_friendly(false),
    m_grazed(false), m_ts(ts), m_unitSpeed(4.5f/1000.0f)
{
    hide();
}

Bullet::~Bullet()
{
    m_staticFields.clear();
    m_dynamicFields.clear();
    m_originItems.clear();
}

void Bullet::setFriendly(bool value)
{
    m_friendly = value;
}

void Bullet::update(int deltaTime)
{
    if (!m_staticFields.empty() || !m_dynamicFields.empty())
    {
        Vector v(0,0);
        for (int i = 0; i < m_staticFields.size(); i++)
        {
            m_staticFields[i]->setOrigin(m_origins[i]);
            v += m_staticFields[i]->getVector(centre());
        }
        int i = 0;
        int size = m_dynamicFields.size();
        while ( i < size && !m_dynamicFields.empty())
        {
            if (m_originItems[i]->isVisible())
            {
                m_dynamicFields[i]->setOrigin(m_originItems[i]->centre());
                v += m_dynamicFields[i]->getVector(centre());
            }
            else
            {
                m_dynamicFields.removeAt(i);
                m_originItems.removeAt(i);
                i--;
                size--;
            }
            i++;
        }
        setVector(v);
    }
    GameItem::setPos(x() + m_unitSpeed * i() * GLOBAL::ObjectLength * deltaTime,
                     y() + m_unitSpeed * j() * GLOBAL::ObjectLength * deltaTime);
    setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);
    setRotation(angle());
}

void Bullet::setPos(const QPointF &pos)
{
    GameItem::setPos(pos.x()-boundingRect().width()/2, pos.y()-boundingRect().height()/2);
}

void Bullet::setPos(qreal x, qreal y)
{
    GameItem::setPos(x-boundingRect().width()/2, y-boundingRect().height()/2);
}

void Bullet::setUnitSpeed(qreal spd)
{
    m_unitSpeed = spd;
}

void Bullet::setDmg(int dmg)
{
    m_dmg = dmg;
}

void Bullet::addField(VectorField *field, QPointF origin)
{
    m_staticFields.append(field);
    m_origins.append(origin);
}

void Bullet::addField(VectorField *field, qreal x, qreal y)
{
    m_staticFields.append(field);
    m_origins.append(QPointF(x,y));
}

void Bullet::addField(VectorField *field, GameItem* originItem)
{
    m_dynamicFields.append(field);
    m_originItems.append(originItem);
}

void Bullet::hide()
{
    m_staticFields.clear();
    m_dynamicFields.clear();
    m_origins.clear();
    m_originItems.clear();
    setVector(0,0);
    m_unitSpeed = 4.5f/1000.0f;
    m_dmg = 1;
    m_friendly = false;
    setBulletID(182);
    m_grazed = false;
    QGraphicsPixmapItem::hide();
}

void Bullet::setBulletID(int id)
{
    setPixmap(m_ts->getInfo(id)->m_pixmap);
}

bool Bullet::collided()
{
    if (!collidesWithItem(parentItem()))
    {
        return true;
    }
    QList<QGraphicsItem*> list = collidingItems();
    for (const auto c : list)
    {
        if (typeid(*c) == typeid(Hitbox) &&
            dynamic_cast<Hitbox*>(c)->m_solid)
        {
            return true;
        }
        else if (m_friendly && typeid(*c) == typeid(Enemy))
        {
            dynamic_cast<Enemy*>(c)->takeDmg(m_dmg);
            return true;
        }
        else if (!m_friendly && typeid(*c) == typeid(Player))
        {
            Player* p = dynamic_cast<Player*>(c);
            if (collidesWithItem(p->hitboxItem()))
            {
                p->takeHit();
                return true;
            }
            else if (!m_grazed)
            {
                p->graze();
                m_grazed = true;
            }
        }
    }
    return false;
}

bool Bullet::isFriendly() const
{
    return m_friendly;
}

qreal Bullet::unitSpeed() const
{
    return m_unitSpeed;
}

int Bullet::dmg() const
{
    return m_dmg;
}
