#include "player.h"

#include "../Utils/global.h"

Player::Player(Database* db)
{
    m_db = db;
    for (int n = 0; n < 4; n++)
    {
        m_party.append(nullptr);
    }
    m_party[0] = m_db->getMovingSprite(2);
    m_activeCharacterIndex = 0;
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
}

void Player::setCharacter(int partyIndex, int SpriteID)
{
    m_party[partyIndex] = m_db->getMovingSprite(SpriteID);
}

MovingSprite* Player::activeCharacter()
{
    return m_party[m_activeCharacterIndex];
}
