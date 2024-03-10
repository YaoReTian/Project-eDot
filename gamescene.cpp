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
    m_tilemap = new Tilemap();
    m_tilemap->setMap(2);
    m_tilemap->generateTiles(*this);
    m_tilemap->generateSprites(*this);
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();
    m_tilemap->update(m_deltaTime);
}
