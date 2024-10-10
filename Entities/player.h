#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "sprite.h"
#include "bulletmanager.h"
#include "../Utils/keymap.h"

class Player : public Sprite
{
public:
    Player(QGraphicsItem * parent = 0);
    ~Player();

    void input(KeyMap* keys);
    virtual void update(int deltaTime) override;
    void setBulletManager(BulletManager* manager);

private:
    int m_elapsedTime;
    bool m_hitboxVisible;
    BulletManager* m_bulletManager;
};

#endif // PLAYER_H
