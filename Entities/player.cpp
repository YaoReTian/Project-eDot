#include "player.h"

#include "../Utils/global.h"
#include "sprite.h"
#include "bullet.h"

Player::Player(QGraphicsItem * parent)
    : Sprite(parent), m_elapsedTime(0), m_hitboxVisible(false)
{

}

Player::~Player()
{

}

void Player::update(int deltaTime)
{
    m_elapsedTime += deltaTime;
    Sprite::update(deltaTime);
}

void Player::input(KeyMap* keys)
{
    bool actionTaken = false;
    if (keys->keyHeldStatus(GLOBAL::SHOOT))
    {
        if (m_elapsedTime > 20)
        {
            m_elapsedTime = 0;
            Bullet* b = new Bullet(parentItem());
            b->setI(prevActiveVector()->i()*2);
            b->setJ(prevActiveVector()->j()*2);
            b->setPos(x()+boundingRect().width()/2 -  b->boundingRect().width()/2,
                      y()+boundingRect().width()/2 - b->boundingRect().width()/2);
            m_bulletManager->addBullet(b);
        }
    }
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

void Player::setBulletManager(BulletManager *manager)
{
    m_bulletManager = manager;
}
