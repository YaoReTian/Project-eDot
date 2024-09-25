#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "sprite.h"
#include "../Utils/keymap.h"

class Player : public Sprite
{
public:
    Player(QGraphicsItem * parent = 0);
    ~Player();

    void input(KeyMap* keys);

private:
    bool m_hitboxVisible;
};

#endif // PLAYER_H
