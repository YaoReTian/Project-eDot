#include "gamescene.h"

#include <QDebug>

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void GameScene::testOutput()
{
    qDebug() << "Testing";
}
