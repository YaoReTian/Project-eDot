#include "player.h"

#include "global.h"

Player::Player(Database* db)
{
    m_db = db;
    for (int n = 0; n < 4; n++)
    {
        m_party.append(nullptr);
    }
    m_party[0] = m_db->getMovingSprite(1);
    m_activeCharacterIndex = 0;
}

void Player::update(int deltatime, KeyMap* keys)
{
    if (keys->getHeldStatus(GLOBAL::MOVE_LEFT))
    {
        m_party[m_activeCharacterIndex]->move(deltatime, GLOBAL::MOVE_LEFT);
    }
    if (keys->getHeldStatus(GLOBAL::MOVE_RIGHT))
    {
        m_party[m_activeCharacterIndex]->move(deltatime, GLOBAL::MOVE_RIGHT);
    }
    if (keys->getHeldStatus(GLOBAL::MOVE_UP))
    {
        m_party[m_activeCharacterIndex]->move(deltatime, GLOBAL::MOVE_UP);
    }
    if (keys->getHeldStatus(GLOBAL::MOVE_DOWN))
    {
        m_party[m_activeCharacterIndex]->move(deltatime, GLOBAL::MOVE_DOWN);
    }
    m_party[m_activeCharacterIndex]->update(deltatime);
}

void Player::setCharacter(int partyIndex, int SpriteID)
{
    m_party[partyIndex] = m_db->getMovingSprite(SpriteID);
}

MovingSprite* Player::getActiveCharacter()
{
    return m_party[m_activeCharacterIndex];
}
