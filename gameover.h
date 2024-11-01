#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include "gameobject.h"

class GameOver : public QGraphicsRectItem, public GameObject
{
public:
    GameOver(QGraphicsItem* parent = 0);
    ~GameOver();

    virtual void update(int deltatime) override;
private:
    QGraphicsPixmapItem* m_text;
};

#endif // GAMEOVER_H
