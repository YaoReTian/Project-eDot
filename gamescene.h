#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QApplication>

#include "tilemap.h"
#include "Utils/keymap.h"
#include "database.h"
#include "Entities/player.h"
#include "menu.h"
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
    void start();
    void newGame();
    void saveSlots();
    void loadGame(int slotID);
    void gameOver();
    void endGame();
    void quit();
    void restartFromCheckpoint();
    void pause();
    void unpause();
    void saveMenu();
    void save(int slotID);

private:

    Tilemap *m_tilemap;
    Database *m_db;
    Player *m_player;
    BulletManager *m_bulletManager;
    ButtonManager *m_buttonManager;
    double m_cameraPosX;
    double m_cameraPosY;

    Menu* m_gameOver;
    Menu* m_start;
    Menu* m_pause;
    Menu* m_loadGame;
    Menu* m_saveGame;
};

#endif // GAMESCENE_H
