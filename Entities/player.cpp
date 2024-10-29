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
            Bullet* b1 = m_bulletManager->getBulletFromPool();
            Bullet* b2 = m_bulletManager->getBulletFromPool();
            Vector v(boundingRect().height()/2, boundingRect().height()/2);
            qreal a = prevActiveVector().angle();
            b1->setPos(centre().x() + v.i()*qCos(a) + prevActiveVector().i(),
                       centre().y() + v.j()*qSin(a) + prevActiveVector().j());
            b2->setPos(centre().x() - v.i()*qCos(a) + prevActiveVector().i(),
                       centre().y() - v.j()*qSin(a) + prevActiveVector().j());
            b1->setUnitSpeed(4.5f/1000.0f);
            b2->setUnitSpeed(4.5f/1000.0f);
            VectorField* f1 = m_bulletManager->getField("UniformPositiveRadial");
            VectorField* f2 = m_bulletManager->getField("UniformPositiveRadial");
            b1->show();
            b2->show();
            b1->addField(f1,centre().x() + v.i()*qCos(a), centre().y() + v.i()*qSin(a));
            b2->addField(f2,centre().x() - v.j()*qCos(a), centre().y() - v.j()*qSin(a));
            m_bulletManager->addBullet(b1);
            m_bulletManager->addBullet(b2);
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
