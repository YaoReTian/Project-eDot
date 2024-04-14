#ifndef MOVINGSPRITE_H
#define MOVINGSPRITE_H

#include "interactivesprite.h"

class MovingSprite : public InteractiveSprite
{
public:
    MovingSprite();
    void update(int deltatime);
    void setAction(int deltatime, GLOBAL::Action action = GLOBAL::NONE);
    void setDefaultToWalk();
    void setDefaultToRun();
    QPointF prevPos();
    bool collidedWithWall();

private:
    float m_currentSpeed;
    float m_defaultSpeed;
    const float m_WALK_SPEED = 3.0f/1000.0f;
    const float m_RUN_SPEED = 3.0f/1000.0f;
    const float m_SPRINT_SPEED = 3.0f/1000.0f;
    float m_velocityX, m_velocityY;
    QPointF m_previousPos;
};

#endif // MOVINGSPRITE_H
