#ifndef MOVINGSPRITE_H
#define MOVINGSPRITE_H

#include "sprite.h"

class MovingSprite : public Sprite
{
public:
    MovingSprite();
    void setAction(int deltatime, GLOBAL::Action action = GLOBAL::NONE);
    void setDefaultToWalk();
    void setDefaultToRun();

private:
    float m_currentSpeed;
    float m_defaultSpeed;
    const float m_WALK_SPEED = 0.8f/1000.0f;
    const float m_RUN_SPEED = 1.3f/1000.0f;
    const float m_SPRINT_SPEED = 2.0f/1000.0f;
    float m_velocityX, m_velocityY;
};

#endif // MOVINGSPRITE_H
