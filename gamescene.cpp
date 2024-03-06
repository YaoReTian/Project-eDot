#include "gamescene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>

#include "tilemap.h"
#include "global.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
{
    // Timer to handle game loop
    testOutput();
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();
    qDebug() << m_deltaTime;

}

void GameScene::testOutput()
{
    Tilemap tilemap(2, *this);
}
