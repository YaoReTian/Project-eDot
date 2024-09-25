#include "player.h"

#include "../Utils/global.h"
#include "sprite.h"

Player::Player(QGraphicsItem * parent)
    : Sprite(parent), m_hitboxVisible(false)
{

}

Player::~Player()
{

}

void Player::input(KeyMap* keys)
{
    bool actionTaken = false;
    if (keys->keyHeldStatus(GLOBAL::MOVE_LEFT))
    {
        actionTaken = true;
        setAction(GLOBAL::MOVE_LEFT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_RIGHT))
    {
        actionTaken = true;
        setAction(GLOBAL::MOVE_RIGHT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_UP))
    {
        actionTaken = true;
        setAction(GLOBAL::MOVE_UP);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_DOWN))
    {
        actionTaken = true;
        setAction(GLOBAL::MOVE_DOWN);
    }
    if (keys->keyHeldStatus(GLOBAL::SHOW_HITBOX))
    {
        m_hitboxVisible = true;
        showHitbox();
    }
    else if (m_hitboxVisible)
    {
        m_hitboxVisible = false;
        hideHitbox();
    }
    if (!actionTaken)
    {
        setAction(GLOBAL::NONE);
    }
}
