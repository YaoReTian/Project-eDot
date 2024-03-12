#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "movingsprite.h"
#include "keymap.h"
#include "database.h"

class Player
{
public:
    Player(Database* db);

    void editParty();
    void update(int deltatime, KeyMap* keys);
    void setCharacter(int partyIndex, int SpriteID);
    MovingSprite* getActiveCharacter();

private:
    Database *m_db;
    QList<MovingSprite*> m_party;
    int m_activeCharacterIndex;
};

#endif // PLAYER_H
