#include "player.h"

#include "../Utils/global.h"
#include "sprite.h"
#include "bullet.h"
#include "../tileset.h"

Player::Player(QGraphicsItem * parent)
    : Sprite(parent), m_username("Default"), m_HP(10), m_maxCharge(50), m_charge(m_maxCharge),
    m_elapsedTime(0), m_hitboxVisible(false), m_immune(false), m_immuneTime(0), m_totalImmuneTime(0),
    m_frozen(false), m_tutorialMode(false), m_tutorialNum(0)
{
    m_healthBar = new ProgressBar(this);
    m_healthBar->setMaximum(m_HP);
    m_healthBar->setValue(m_HP);
    m_chargeBar = new ProgressBar(this);
    m_chargeBar->setMaximum(m_maxCharge);
    m_chargeBar->setValue(m_charge);
    m_chargeBar->setAllignment(Vertical);
    m_chargeBar->setProgressColour(Qt::blue);
    m_chargeBar->setProgressLowColour(Qt::blue);
    TileSet t(":/tileset/res/Basic bullets.tsj",1);
    QPixmap h = t.getInfo(40)->m_pixmap;
    m_hitbox.setPixmap(h.scaledToHeight(h.height()*GLOBAL::Scale/4));
    m_hitbox.setParentItem(this);
    m_hitbox.setOpacity(0);
    m_hitbox.setPos(boundingRect().width()/2 - m_hitbox.boundingRect().width()/2,
                    boundingRect().height()/2 - m_hitbox.boundingRect().height()/2);
    setEngaged(true);
}

Player::~Player()
{
    delete m_healthBar;
    delete m_chargeBar;
}

void Player::setParentItem(QGraphicsItem *parent)
{
    Sprite::setParentItem(parent);
    m_hitbox.setParentItem(parent);
}

void Player::update(int deltaTime)
{
    m_elapsedTime += deltaTime;
    m_hitbox.setZValue(zValue()+1);
    m_hitbox.setPos(boundingRect().width()/2 - m_hitbox.boundingRect().width()/2,
                    boundingRect().height()/2 - m_hitbox.boundingRect().height()/2);
    m_healthBar->setValue(m_HP);
    m_healthBar->setZValue(zValue() + 1);
    m_healthBar->setRect(-GLOBAL::Scale, -6*GLOBAL::Scale, (frameSize().width()+2)*GLOBAL::Scale, 5*GLOBAL::Scale);
    m_healthBar->update(deltaTime);
    m_chargeBar->setValue(m_charge);
    m_chargeBar->setZValue(zValue() + 1);
    m_chargeBar->setRect(-6*GLOBAL::Scale, -GLOBAL::Scale, 5*GLOBAL::Scale, (frameSize().height()+2)*GLOBAL::Scale);
    m_chargeBar->update(deltaTime);
    if (m_HP == 0 && animationDone())
    {
        setOpacity(1.0);
        emit dead();
        return;
    }
    Sprite::update(deltaTime);

    if (m_immune)
    {
        m_immuneTime += deltaTime;
        if (m_totalImmuneTime == 5)
        {
            m_immune = false;
            m_immuneTime = 0;
            m_totalImmuneTime = 0;
            setOpacity(1.0);
        }
        else if (m_immuneTime >= 300)
        {
            m_totalImmuneTime++;
            m_immuneTime = 0;
            if (opacity() == 1.0)
            {
                setOpacity(0.2);
            }
            else
            {
                setOpacity(1.0);
            }
        }
    }
}

void Player::setHitboxPixmap(QPixmap p)
{
    m_hitbox.setPixmap(p);
}

void Player::input(KeyMap* keys)
{
    if (m_HP == 0)
    {
        setAction(GLOBAL::DEAD);
        return;
    }
    if (m_frozen)
    {
        return;
    }
    bool actionTaken = false;
    if (keys->keyHeldStatus(GLOBAL::SHOOT) && !(m_tutorialMode && m_tutorialNum < 2))
    {
        if (m_elapsedTime > 50)
        {
            emit shooting();
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
            m_bulletManager->addBullet(b1, true);
            m_bulletManager->addBullet(b2, true);
        }
    }
    if (keys->keyHeldStatus(GLOBAL::OVERDRIVE) && !(m_tutorialMode && m_tutorialNum < 3))
    {
        if (m_charge < m_maxCharge)
        {
            emit message("Not enough charge", Message::timeout);
        }
        else if (m_charge == m_maxCharge)
        {
            emit overdriveUsed();
            m_charge = 0;
            m_bulletManager->overdrive();
            setAction(GLOBAL::OVERDRIVE);
        }
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_LEFT))
    {
        actionTaken = true;
        emit moved();
        setAction(GLOBAL::MOVE_LEFT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_RIGHT))
    {
        actionTaken = true;
        emit moved();
        setAction(GLOBAL::MOVE_RIGHT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_UP))
    {
        actionTaken = true;
        emit moved();
        setAction(GLOBAL::MOVE_UP);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_DOWN))
    {
        actionTaken = true;
        emit moved();
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
    if (keys->keyHeldStatus(GLOBAL::SHOW_PLAYER_HITBOX) && !(m_tutorialMode && m_tutorialNum < 1))
    {
        emit slowMoved();
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
    if (m_HP > 10)
    {
        m_HP = 10;
    }
    setAction(GLOBAL::HEAL);
}

void Player::takeHit()
{
    if (m_immune)
    {
        return;
    }
    else
    {
        setOpacity(0.2);
        m_immune = true;
    }
    if (m_HP > 0)
    {
        m_HP--;
    }
    if (m_HP == 0)
    {
        setAction(GLOBAL::DEAD);
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
    m_elapsedTime = 0;
    heal(10);
    m_charge = m_maxCharge;
    m_immune = false;
    m_immuneTime = 0;
    m_totalImmuneTime = 0;
    setOpacity(1.0);
    m_tutorialMode = false;
    m_tutorialNum = 0;
}

void Player::setUsername(QString name)
{
    m_username = name;
}

QString Player::username()
{
    return m_username;
}

void Player::graze()
{
    if (m_charge < m_maxCharge)
    {
        m_charge++;
    }
}

void Player::setCharge(int val)
{
    if (val > m_maxCharge)
    {
        m_charge = m_maxCharge;
    }
    else
    {
        m_charge = val;
    }
}

int Player::charge() const
{
    return m_charge;
}

int Player::maxCharge() const
{
    return m_maxCharge;
}

// SLOTS

void Player::freeze()
{
    m_frozen = true;
}

void Player::unfreeze()
{
    m_frozen = false;
}

void Player::setTutorialMode(bool value)
{
    m_tutorialMode = value;
}

void Player::nextTutorial()
{
    if (m_tutorialMode)
    {
        m_tutorialNum++;
        if (m_tutorialNum == 4)
        {
            m_tutorialNum = 0;
            m_tutorialMode = false;
        }
    }
}
