#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>

#include "tilemap.h"
#include "Utils/keymap.h"
#include "database.h"
#include "Entities/player.h"
#include "UI/userinterface.h"

class WorldScene : public QGraphicsScene
{
    Q_OBJECT
public:
    WorldScene(QObject *parent = 0);
    void loop();
    void clear();

private:
    QTimer m_timer;
    Tilemap *m_tilemap;
    KeyMap *m_keymap;
    Database *m_db;
    Player *m_player;
    UserInterface *m_UI;
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f;

protected:
    // Overridden functions
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // GAMESCENE_H
