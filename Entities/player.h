#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "combatsprite.h"
#include "database.h"

class Player
{
public:
    Player(Database* db);

    void editParty();

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime, KeyMap* keys);
    void render(QGraphicsScene &scene);

    bool enteredCombat();

    void setCharacter(int partyIndex, int SpriteID);
    CombatSprite* activeCharacter();
    QList<CombatSprite*> getParty();

private:
    bool m_enteredCombat;
    Database *m_db;
    QList<CombatSprite*> m_party;
    int m_activeCharacterIndex;
};

#endif // PLAYER_H
