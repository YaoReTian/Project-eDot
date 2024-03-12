#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>

#include "tilemap.h"
#include "keymap.h"
#include "database.h"
#include "player.h"

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);
    void loop();
    void testOutput();
    void clear();

private:
    QTimer m_timer;
    Tilemap *m_tilemap;
    KeyMap *m_keymap;
    Database *m_db;
    Player *m_player;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f;

protected:
    // Overridden functions
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
