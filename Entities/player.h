#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "combatsprite.h"
#include "../database.h"
#include "../gameobject.h"

class Player : public GameObject
{
public:
    Player(Database* db);
    ~Player();

    void editParty();

    void input(KeyMap* keys);
    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

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
