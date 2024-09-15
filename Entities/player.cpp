#include "player.h"

#include "../Utils/global.h"
#include "sprite.h"

Player::Player(Database* db)
    : m_enteredCombat(false), m_db(db)
{
    m_tileset = new TileSet(":/tileset/res/General.tsj",1);
    TileInfo* sprite = m_tileset->getInfo(1);
    m_sprite = m_db->getSprite(sprite->m_path);
    for (const auto h : std::as_const(sprite->m_hitboxes))
    {
        m_sprite->setHitbox(h);
    }
}

Player::~Player()
{
    delete m_sprite;
}

void Player::input(KeyMap* keys)
{
    bool actionTaken = false;
    if (keys->keyHeldStatus(GLOBAL::MOVE_LEFT))
    {
        actionTaken = true;
        m_sprite->setAction(GLOBAL::MOVE_LEFT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_RIGHT))
    {
        actionTaken = true;
        m_sprite->setAction(GLOBAL::MOVE_RIGHT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_UP))
    {
        actionTaken = true;
        m_sprite->setAction(GLOBAL::MOVE_UP);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_DOWN))
    {
        actionTaken = true;
        m_sprite->setAction(GLOBAL::MOVE_DOWN);
    }
    if (!actionTaken)
    {
        m_sprite->setAction(GLOBAL::NONE);
    }
}

void Player::clear(QGraphicsScene &scene)
{
    m_sprite->clear(scene);
}

void Player::update(int deltatime)
{
    m_sprite->update(deltatime);
}

void Player::render(QGraphicsScene &scene)
{
    m_sprite->render(scene);
}

bool Player::enteredCombat()
{
    return m_enteredCombat;
}

void Player::setPos(int x, int y)
{
    m_sprite->setPos(x, y);
}

void Player::setZValue(float zValue)
{
    m_sprite->setZValue(zValue);
}

void Player::setSprite(Sprite* sprite)
{
    m_sprite = sprite;
}

Sprite* Player::getSprite()
{
    return m_sprite;
}
