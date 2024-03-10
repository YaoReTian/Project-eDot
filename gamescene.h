#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QElapsedTimer>

#include "tilemap.h"
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
    QElapsedTimer m_elapsedTimer;
    float m_deltaTime = 0.0f;
};

#endif // GAMESCENE_H
