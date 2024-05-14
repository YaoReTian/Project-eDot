#include "movingsprite.h"

#include "../tile.h"

MovingSprite::MovingSprite()
    : InteractiveSprite(), m_WALK_SPEED(1.5f/1000.0f), m_RUN_SPEED(3.0f/1000.0f),
    m_SPRINT_SPEED(4.5f/1000.0f), m_defaultSpeed(m_RUN_SPEED), m_currentSpeed(m_defaultSpeed),
    m_velocityX(0), m_velocityY(0), m_previousPos(0,0)
{
}

MovingSprite::~MovingSprite() {}

void MovingSprite::update(int deltatime)
{
    InteractiveSprite::update(deltatime);

    // Check wall collisions
    for (int i = 0; i < 10; i++)
    {
        m_previousPos = pos();
        setX(x()+(m_velocityX * deltatime)/10);
        if (collidedWithWall()) setPos(m_previousPos);
    }
    for (int i = 0; i < 10; i++)
    {
        m_previousPos = pos();
        setY(y()+(m_velocityY * deltatime)/10);
        if (collidedWithWall()) setPos(m_previousPos);
    }

    setZValue(GLOBAL::SPRITE_LAYER + y());
    m_velocityX = 0;
    m_velocityY = 0;
}

void MovingSprite::setAction(GLOBAL::Action action)
{
    if (isInteractingWithPlayer()) action = GLOBAL::NONE;

    InteractiveSprite::setAction(action);
    if (action == GLOBAL::MOVE_LEFT)
    {
        m_velocityX = -m_currentSpeed * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::MOVE_RIGHT)
    {
        m_velocityX = m_currentSpeed * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::MOVE_UP)
    {
        m_velocityY = -m_currentSpeed * GLOBAL::ObjectLength;
    }
    else if (action == GLOBAL::MOVE_DOWN)
    {
        m_velocityY = m_currentSpeed * GLOBAL::ObjectLength;
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

bool MovingSprite::collidedWithWall()
{
    QList<QGraphicsItem*> list = collidingItems();

    for (const auto s : list)
    {
        if (typeid(*s) == typeid(Tile) &&
            dynamic_cast<Tile*>(s)->isSolid() &&
            s->y() + s->boundingRect().height() + 3*GLOBAL::Scale >= y()+boundingRect().height())
        {
            return true;
        }
    }
    return false;
}
