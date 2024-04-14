#include "player.h"

#include "../Utils/global.h"
#include "interactivesprite.h"

Player::Player(Database* db)
{
    m_db = db;
    for (int n = 0; n < 4; n++)
    {
        m_party.append(nullptr);
    }
    m_party[0] = dynamic_cast<CombatSprite*>(m_db->getSprite(1));
    m_party[0]->setSide(Friendly);
    m_party[0]->setBaseStat(SPD, 100);
    m_activeCharacterIndex = 0;
    m_enteredCombat = true;
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
        if (typeid(*s) == typeid(InteractiveSprite))
        {
            InteractiveSprite* sprite = static_cast<InteractiveSprite*>(s);
            if (sprite->isInteractive())   sprite->popup();
        }
        else if (typeid(*s) == typeid(MovingSprite))
        {
            MovingSprite* sprite = static_cast<MovingSprite*>(s);
            if (sprite->isInteractive())   sprite->popup();
        }
        else if (typeid(*s) == typeid(CombatSprite))
        {
            CombatSprite* sprite = static_cast<CombatSprite*>(s);
            if (sprite->isInteractive())   sprite->popup();
        }
    }
}

void Player::render(QGraphicsScene &scene)
{
    activeCharacter()->render(scene);
}

bool Player::enteredCombat()
{
    return m_enteredCombat;
}

void Player::setCharacter(int partyIndex, int SpriteID)
{
    m_party[partyIndex] = dynamic_cast<CombatSprite*>(m_db->getSprite(SpriteID));
}

CombatSprite* Player::activeCharacter()
{
    return m_party[m_activeCharacterIndex];
}

QList<CombatSprite*> Player::getParty()
{
    QList<CombatSprite*> list;

    for (const auto p : m_party)
    {
        if (p != nullptr)   list.append(p);
    }

    return list;
}
