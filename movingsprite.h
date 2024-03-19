#ifndef MOVINGSPRITE_H
#define MOVINGSPRITE_H

#include "sprite.h"
#include "global.h"

class MovingSprite : public Sprite
{
public:
    MovingSprite();
    void setAction(int deltatime, GLOBAL::Action action = GLOBAL::NONE);
private:
    bool m_sprinting;
    const float m_WALK_SPEED = 1.3f/1000.0f;
    const float m_SPRINT_SPEED = 2.0f/1000.0f;
    float m_velocityX, m_velocityY;
};

#endif // MOVINGSPRITE_H
