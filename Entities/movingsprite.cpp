#include "movingsprite.h"

#include "../tile.h"

MovingSprite::MovingSprite() : Sprite()
{
    m_defaultSpeed = m_RUN_SPEED;
    m_currentSpeed = m_defaultSpeed;
    m_previousPos = QPointF(0,0);
}

void MovingSprite::update(int deltatime)
{
    Sprite::update(deltatime);

    // Check wall collisions
    for (int i = 0; i < 10; i++)
    {
        m_previousPos = pos();
        setX(x()+m_velocityX/10);
        if (collidedWithWall()) setPos(m_previousPos);
    }
    for (int i = 0; i < 10; i++)
    {
        m_previousPos = pos();
        setY(y()+m_velocityY/10);
        if (collidedWithWall()) setPos(m_previousPos);
    }

    setZValue(GLOBAL::SPRITE_LAYER + y());
    m_velocityX = 0;
    m_velocityY = 0;
}

void MovingSprite::setAction(int deltatime, GLOBAL::Action action)
{
    if (isInteractingWithPlayer()) action = GLOBAL::NONE;

    Sprite::setAction(action);
    if (action == GLOBAL::MOVE_LEFT)
    {
        m_velocityX = -m_currentSpeed * deltatime * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::MOVE_RIGHT)
    {
        m_velocityX = m_currentSpeed * deltatime * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::MOVE_UP)
    {
        m_velocityY = -m_currentSpeed * deltatime * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::MOVE_DOWN)
    {
        m_velocityY = m_currentSpeed * deltatime * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::SPRINT)
    {
        m_currentSpeed = (m_currentSpeed == m_SPRINT_SPEED) ? m_defaultSpeed : m_SPRINT_SPEED;
    }
    else if (action == GLOBAL::WALK)
    {
        m_currentSpeed = (m_currentSpeed == m_WALK_SPEED) ? m_defaultSpeed : m_SPRINT_SPEED;
    }
}

void MovingSprite::setDefaultToWalk()
{
    m_defaultSpeed = m_WALK_SPEED;
    m_currentSpeed = m_defaultSpeed;
}

void MovingSprite::setDefaultToRun()
{
    m_defaultSpeed = m_RUN_SPEED;
    m_currentSpeed = m_defaultSpeed;
}

QPointF MovingSprite::prevPos()
{
    return m_previousPos;
}

bool MovingSprite::collidedWithWall()
{
    QList<QGraphicsItem*> list = collidingItems();

    for (const auto s : list)
    {
        if (typeid(*s) == typeid(Tile))
        {
            if (dynamic_cast<Tile*>(s)->isSolid() &&
                s->y() + s->boundingRect().height() + 3*GLOBAL::Scale >= y()+boundingRect().height())
            {
                return true;
            }
        }
    }
    return false;
}
