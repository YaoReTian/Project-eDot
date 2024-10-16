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
    delete m_bulletManager;
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
            b->setVector(0,0);
            b->setPos(x()+boundingRect().width()/2 - b->boundingRect().width()/2 + prevActiveVector().i(),
                      y()+boundingRect().height()/2 - b->boundingRect().height()/2 + prevActiveVector().j());
            b->setUnitSpeed(4.5f/1000.0f);
            VectorField* f = new VectorField("x/sqrt(x^2 + y^2)","y/sqrt(x^2 + y^2)");
            f->setOrigin(x()+boundingRect().width()/2, y()+boundingRect().height()/2);
            BulletField *bf = new BulletField;
            bf->m_bullets.append(b);
            bf->m_fields.append(f);
            m_bulletManager->addBulletField(bf);
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
