#include "movingsprite.h"

MovingSprite::MovingSprite() : Sprite()
{
    m_sprinting = false;
    setType("MovingSprite");
}

void MovingSprite::setAction(int deltatime, GLOBAL::Action action)
{
    m_velocityX = 0;
    m_velocityY = 0;
    Sprite::setAction(action);
    if (action == GLOBAL::MOVE_LEFT)
    {
        m_velocityX = -(m_sprinting ? m_SPRINT_SPEED : m_WALK_SPEED) * deltatime * GLOBAL::ObjectLength;
    }
    if (action == GLOBAL::MOVE_RIGHT)
    {
        m_velocityX = (m_sprinting ? m_SPRINT_SPEED : m_WALK_SPEED) * deltatime * GLOBAL::ObjectLength;
    }
    if (action == GLOBAL::MOVE_UP)
    {
        m_velocityY = -(m_sprinting ? m_SPRINT_SPEED : m_WALK_SPEED) * deltatime * GLOBAL::ObjectLength;
    }
    if (action == GLOBAL::MOVE_DOWN)
    {
        m_velocityY = (m_sprinting ? m_SPRINT_SPEED : m_WALK_SPEED) * deltatime * GLOBAL::ObjectLength;
    }
    setPos((pos().x()+m_velocityX), (pos().y()+m_velocityY));
}
