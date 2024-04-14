#ifndef MOVINGSPRITE_H
#define MOVINGSPRITE_H

#include "interactivesprite.h"

class MovingSprite : public InteractiveSprite
{
public:
    MovingSprite();
    ~MovingSprite();
    virtual void update(int deltatime) override;
    void setAction(GLOBAL::Action action = GLOBAL::NONE);
    void setDefaultToWalk();
    void setDefaultToRun();
    QPointF prevPos();
    bool collidedWithWall();

private:
    const float m_WALK_SPEED;
    const float m_RUN_SPEED;
    const float m_SPRINT_SPEED;
    float m_defaultSpeed;
    float m_currentSpeed;
    float m_velocityX, m_velocityY;
    QPointF m_previousPos;
};

#endif // MOVINGSPRITE_H
