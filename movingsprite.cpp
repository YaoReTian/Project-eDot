#include "movingsprite.h"

MovingSprite::MovingSprite() : Sprite()
{
    m_defaultSpeed = m_RUN_SPEED;
    m_currentSpeed = m_defaultSpeed;
    setType("MovingSprite");
}

void MovingSprite::setAction(int deltatime, GLOBAL::Action action)
{
    m_velocityX = 0;
    m_velocityY = 0;
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
    setPos((pos().x()+m_velocityX), (pos().y()+m_velocityY));
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
