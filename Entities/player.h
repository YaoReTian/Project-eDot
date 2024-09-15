#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "../database.h"
#include "../gameobject.h"
#include "../Utils/keymap.h"
#include "tileset.h"

class Player : public GameObject
{
public:
    Player(Database* db);
    ~Player();

    void input(KeyMap* keys);
    virtual void clear(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    bool enteredCombat();
    void setPos(int x, int y);
    void setZValue(float z);
    void setSprite(Sprite* sprite);

    Sprite* getSprite();

private:
    bool m_enteredCombat;
    Database *m_db;
    Sprite* m_sprite;
    TileSet* m_tileset;
    qreal m_zValue;
};

#endif // PLAYER_H
