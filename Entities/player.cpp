#include "player.h"

#include "../Utils/global.h"
#include "sprite.h"
#include "bullet.h"
#include "../tileset.h"

Player::Player(QGraphicsItem * parent)
    : Sprite(parent), m_username("Default"), m_HP(10), m_elapsedTime(0), m_hitboxVisible(false)
{
    m_healthBar = new ProgressBar(this);
    m_healthBar->setMaximum(m_HP);
    m_healthBar->setValue(m_HP);
    TileSet t(":/tileset/res/Basic bullets.tsj",1);
    m_hitbox.setPixmap(t.getInfo(40)->m_pixmap);
    m_hitbox.setParentItem(parent);
    m_hitbox.setOpacity(0);
}

Player::~Player()
{
    delete m_healthBar;
}

void Player::setParentItem(QGraphicsItem *parent)
{
    Sprite::setParentItem(parent);
    m_hitbox.setParentItem(parent);
}

void Player::update(int deltaTime)
{
    m_elapsedTime += deltaTime;
    Sprite::update(deltaTime);
    m_hitbox.setPos(centre().x() - m_hitbox.boundingRect().width()/2,
                    centre().y() - m_hitbox.boundingRect().height()/2);
    m_hitbox.setZValue(zValue()+1);
    m_healthBar->setValue(m_HP);
    m_healthBar->setZValue(zValue() + 1);
    m_healthBar->setRect(-GLOBAL::Scale, -6*GLOBAL::Scale, (frameSize().width()+2)*GLOBAL::Scale, 5*GLOBAL::Scale);
    m_healthBar->update(deltaTime);
}

void Player::setHitboxPixmap(QPixmap p)
{
    m_hitbox.setPixmap(p);
}

void Player::input(KeyMap* keys)
{
    bool actionTaken = false;
    if (keys->keyHeldStatus(GLOBAL::SHOOT))
    {
        if (m_elapsedTime > 50)
        {
            m_elapsedTime = 0;
            Bullet* b1 = m_bulletManager->getBulletFromPool();
            Bullet* b2 = m_bulletManager->getBulletFromPool();
            Vector v(boundingRect().height()/2, boundingRect().height()/2);
            Vector d(centre(), keys->mousePos());
            d.toUnitVector();
            qreal a = d.angle();
            b1->setPos(centre().x() + v.i()*qCos(a) + d.i(),
                       centre().y() + v.j()*qSin(a) + d.j());
            b2->setPos(centre().x() - v.i()*qCos(a) + d.i(),
                       centre().y() - v.j()*qSin(a) + d.j());
            b1->setUnitSpeed(4.5f/1000.0f);
            b2->setUnitSpeed(4.5f/1000.0f);
            VectorField* f1 = m_bulletManager->getField("UniformPositiveRadial");
            VectorField* f2 = m_bulletManager->getField("UniformPositiveRadial");
            b1->show();
            b2->show();
            b1->addField(f1,centre().x() + v.i()*qCos(a), centre().y() + v.i()*qSin(a));
            b2->addField(f2,centre().x() - v.j()*qCos(a), centre().y() - v.j()*qSin(a));
            b1->setFriendly();
            b2->setFriendly();
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
    if (keys->keyHeldStatus(GLOBAL::SHOW_PLAYER_HITBOX))
    {
        m_hitbox.setOpacity(1);
        setDefaultToWalk();
    }
    else if (m_hitbox.opacity() > 0)
    {
        m_hitbox.setOpacity(0);
        setDefaultToRun();
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

void Player::heal(int hpToAdd)
{
    m_HP += hpToAdd;
}

void Player::takeHit()
{
    m_HP--;
    if (m_HP == 0)
    {
        emit dead();
    }
    if (m_HP < 0)
    {
        m_HP = 0;
    }
}

int Player::HP() const
{
    return m_HP;
}

QGraphicsItem* Player::hitboxItem()
{
    return &m_hitbox;
}

void Player::reset()
{
    m_HP = 10;
}

void Player::setUsername(QString name)
{
    m_username = name;
}

QString Player::username()
{
    return m_username;
}
