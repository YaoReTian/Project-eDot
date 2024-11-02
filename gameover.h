#ifndef GAMEOVER_H
#define GAMEOVER_H

#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include "gameobject.h"
#include "UI/buttonmanager.h"

class GameOver : public QObject, public QGraphicsRectItem, public GameObject
{
    Q_OBJECT
public:
    GameOver(QGraphicsItem* parent = 0);
    ~GameOver();

    virtual void update(int deltatime) override;

private:
    QGraphicsPixmapItem* m_text;
};

#endif // GAMEOVER_H
