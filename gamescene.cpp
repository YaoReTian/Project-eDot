#include "gamescene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

#include "global.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
{
    // Instantiate objects
    m_db = new Database;
    m_tilemap = new Tilemap(m_db);
    m_keymap = new KeyMap();
    m_player = new Player(m_db);

    // Add functionality
    m_keymap->setDefaultBindings();
    m_tilemap->setMap(2);
    m_tilemap->generateTiles(*this);
    m_tilemap->generateSprites(*this);
    m_player->getActiveCharacter()->setPos(m_tilemap->getMapSizeX()/2, m_tilemap->getMapSizeY()/2);
    addItem(m_player->getActiveCharacter());

    // Timer to handle game loop
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();
    m_tilemap->update(m_deltaTime);
    m_player->update(m_deltaTime, m_keymap);
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    Qt::Key key = static_cast<Qt::Key>(event->key());

    if (m_keymap->contains(key))
    {
        GLOBAL::Action action = m_keymap->getAction(key);
        m_keymap->setHeld(action, true);
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    Qt::Key key = static_cast<Qt::Key>(event->key());

    if (m_keymap->contains(key))
    {
        GLOBAL::Action action = m_keymap->getAction(key);
        m_keymap->setHeld(action, false);
        m_keymap->setReleased(action, true);
    }
    QGraphicsScene::keyReleaseEvent(event);
}
