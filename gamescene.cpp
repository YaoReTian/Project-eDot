#include "gamescene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>

#include "tilemap.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void GameScene::testOutput() {
    Tilemap tilemap(2, *this);
}
