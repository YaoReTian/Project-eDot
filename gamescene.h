#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QPainter>

#include "tilemap.h"
#include "Utils/keymap.h"
#include "database.h"
#include "Entities/player.h"
#include "UI/userinterface.h"
#include "turnbased.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);
    ~GameScene();
    void loop();
    void updateCamera();

private:
    enum GameLayer
    {
        WORLD,
        TURN_BASED,
        BULLET_HELL,
        MENU,
        MAIN_MENU,
        PAUSE_MENU
    };

    QTimer m_timer;
    Tilemap *m_tilemap;
    KeyMap *m_keymap;
    Database *m_db;
    Player *m_player;
    UserInterface *m_UI;
    TurnBased* m_turnbased;
    QElapsedTimer m_elapsedTimer;
    GameLayer m_currentLayer;
    float m_deltaTime;
    double m_cameraPosX;
    double m_cameraPosY;

protected:
    // Overridden functions
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
