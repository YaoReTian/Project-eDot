#include "player.h"

#include "../Utils/global.h"

Player::Player(Database* db)
{
    m_db = db;
    for (int n = 0; n < 4; n++)
    {
        m_party.append(nullptr);
    }
    m_party[0] = dynamic_cast<MovingSprite*>(m_db->getSprite(1));
    m_activeCharacterIndex = 0;
}

void Player::removeItem(QGraphicsScene &scene)
{
    activeCharacter()->removeItem(scene);
}

void Player::update(int deltatime, KeyMap* keys)
{
    bool actionTaken = false;
    if (keys->keyHeldStatus(GLOBAL::MOVE_LEFT))
    {
        actionTaken = true;
        activeCharacter()->setAction(deltatime, GLOBAL::MOVE_LEFT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_RIGHT))
    {
        actionTaken = true;
        activeCharacter()->setAction(deltatime, GLOBAL::MOVE_RIGHT);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_UP))
    {
        actionTaken = true;
        activeCharacter()->setAction(deltatime, GLOBAL::MOVE_UP);
    }
    if (keys->keyHeldStatus(GLOBAL::MOVE_DOWN))
    {
        actionTaken = true;
        activeCharacter()->setAction(deltatime, GLOBAL::MOVE_DOWN);
    }
    if (!actionTaken)
    {
        activeCharacter()->setAction(deltatime, GLOBAL::NONE);
    }
    activeCharacter()->update(deltatime);

    QList<QGraphicsItem*> list = activeCharacter()->collidingItems();

    for (const auto s : list)
    {
        if (typeid(*s) == typeid(Sprite))
        {
            dynamic_cast<Sprite*>(s)->popup();
        }
    }
}

void Player::render(QGraphicsScene &scene)
{
    activeCharacter()->render(scene);
}

void Player::setCharacter(int partyIndex, int SpriteID)
{
    m_party[partyIndex] = dynamic_cast<MovingSprite*>(m_db->getSprite(SpriteID));
}

MovingSprite* Player::activeCharacter()
{
    return m_party[m_activeCharacterIndex];
}
