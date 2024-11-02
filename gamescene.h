#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QApplication>

#include "tilemap.h"
#include "Utils/keymap.h"
#include "database.h"
#include "Entities/player.h"
#include "gameover.h"
#include "UI/buttonmanager.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);
    ~GameScene();
    void input(KeyMap* keys);
    void update(int deltatime);
    void updateCamera();

public slots:
    void gameOver();
    void endGame();
    void restartFromCheckpoint();

private:

    Tilemap *m_tilemap;
    Database *m_db;
    Player *m_player;
    BulletManager *m_bulletManager;
    ButtonManager *m_buttonManager;
    double m_cameraPosX;
    double m_cameraPosY;

    GameOver* m_gameOver;
};

#endif // GAMESCENE_H
