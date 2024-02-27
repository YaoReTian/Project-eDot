#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>

class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    GameScene(QObject *parent = 0);
    void testOutput();
};

#endif // GAMESCENE_H
