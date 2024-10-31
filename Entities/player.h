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

    void setParentItem(QGraphicsItem *parent);
    void input(KeyMap* keys);
    virtual void update(int deltaTime) override;
    void setBulletManager(BulletManager* manager);

    void setHitboxPixmap(QPixmap p);

    void heal(int hpToAdd = 0);
    void takeHit();
    int HP() const;
    QGraphicsItem* hitboxItem();

private:
    int m_HP;
    int m_elapsedTime;
    bool m_hitboxVisible;
    BulletManager* m_bulletManager;
    QList<VectorField*> m_fields;
    QGraphicsPixmapItem m_hitbox;
};

#endif // PLAYER_H
