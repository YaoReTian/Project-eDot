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
    m_UI = new UserInterface(this);
    m_keymap = new KeyMap;
    m_tilemap = new Tilemap(m_db, m_UI);
    m_player = new Player(m_db);

    // Add functionality
    m_keymap->setDefaultBindings();
    m_tilemap->setMap(1);
    m_player->activeCharacter()->setPos(0,0);

    addItem(m_player->activeCharacter());
    m_tilemap->generateTiles(*this);
    m_tilemap->generateSprites(*this);

    // Timer to handle game loop
    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();
    m_tilemap->update(m_deltaTime, m_UI, m_player->activeCharacter());
    m_player->update(m_deltaTime, m_keymap);
    m_UI->update(m_keymap, m_player->activeCharacter());

    // Reset mouse and key status
    m_keymap->resetStatus();

}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    Qt::Key key = static_cast<Qt::Key>(event->key());

    if (m_keymap->contains(key))
    {
        GLOBAL::Action action = m_keymap->getAction(key);
        m_keymap->keyHeld(action);
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    Qt::Key key = static_cast<Qt::Key>(event->key());

    if (m_keymap->contains(key))
    {
        GLOBAL::Action action = m_keymap->getAction(key);
        m_keymap->keyHeld(action);
        m_keymap->keyReleased(action);
    }
    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_keymap->mouseHeld();
    QGraphicsScene::mousePressEvent(event);
}

void GameScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_keymap->mouseReleased();
    QGraphicsScene::mouseReleaseEvent(event);
}
