#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>

#include "tilemap.h"
#include "Utils/keymap.h"
#include "database.h"
#include "Entities/player.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);
    ~GameScene();
    void input(KeyMap* keys);
    void update(int deltatime);
    void updateCamera();

private:

    Tilemap *m_tilemap;
    Database *m_db;
    Player *m_player;
    BulletManager *m_bulletManager;
    double m_cameraPosX;
    double m_cameraPosY;
};

#endif // GAMESCENE_H
