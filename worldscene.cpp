#include "worldscene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QKeyEvent>

#include "Utils/global.h"

WorldScene::WorldScene(QObject *parent) :
    QGraphicsScene(parent)
{
    // Instantiate objects
    m_db = new Database;
    m_UI = new UserInterface();
    m_keymap = new KeyMap;
    m_tilemap = new Tilemap(m_db);
    m_player = new Player(m_db);

    // Add functionality
    m_keymap->setDefaultBindings();
    m_tilemap->setMap(3, m_UI);
    m_player->activeCharacter()->setPos(4*GLOBAL::ObjectLength,4*GLOBAL::ObjectLength);

    addItem(m_player->activeCharacter());
    m_tilemap->generateTiles(*this);
    m_tilemap->generateSprites(*this);

    // Timer to handle game loop
    connect(&m_timer, &QTimer::timeout, this, &WorldScene::loop);
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();
}

void WorldScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    // Update
    m_tilemap->update(m_deltaTime);
    m_player->update(m_deltaTime, m_keymap);
    m_UI->update(m_deltaTime, m_keymap, m_player->activeCharacter());

    // Render
    clear();
    m_tilemap->render(*this);
    m_player->render(*this);
    m_UI->render(*this);

    // Reset mouse and key status
    m_keymap->resetStatus();

}

void WorldScene::clear()
{
    m_tilemap->removeItem(*this);
    m_player->removeItem(*this);
    m_UI->removeItem(*this);
}

void WorldScene::keyPressEvent(QKeyEvent *event)
{
    Qt::Key key = static_cast<Qt::Key>(event->key());
    if (m_keymap->contains(key))
    {
        GLOBAL::Action action = m_keymap->getAction(key);
        m_keymap->keyHeld(action);
    }
    QGraphicsScene::keyPressEvent(event);
}

void WorldScene::keyReleaseEvent(QKeyEvent *event)
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

void WorldScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    m_keymap->mouseHeld();
    QGraphicsScene::mousePressEvent(event);
}

void WorldScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_keymap->mouseReleased();
    QGraphicsScene::mouseReleaseEvent(event);
}
