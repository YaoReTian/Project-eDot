#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "movingsprite.h"
#include "database.h"

class Player
{
public:
    Player(Database* db);

    void editParty();

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime, KeyMap* keys);
    void render(QGraphicsScene &scene);

    void setCharacter(int partyIndex, int SpriteID);
    MovingSprite* activeCharacter();

private:
    Database *m_db;
    QList<MovingSprite*> m_party;
    int m_activeCharacterIndex;
};

#endif // PLAYER_H
