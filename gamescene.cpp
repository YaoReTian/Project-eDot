#include "gamescene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QImage>

#include "Utils/global.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent), m_currentLayer(WORLD), m_deltaTime(0.0f), m_cameraPosX(0), m_cameraPosY(0)
{
    // Instantiate objects
    m_db = new Database;
    m_UI = new UserInterface;
    m_keymap = new KeyMap;
    m_tilemap = new Tilemap;
    m_player = new Player(m_db);

    m_tilemap->setDatabase(m_db);
    m_tilemap->setMap(3, m_UI);
    addItem(m_tilemap);

    m_player->getSprite()->setParentItem(m_tilemap);
    m_player->setZValue(m_tilemap->getPlayerZ());
    m_keymap->setWorldBindings();
    m_player->setPos(4*GLOBAL::ObjectLength,4*GLOBAL::ObjectLength);

    connect(&m_timer, &QTimer::timeout, this, &GameScene::loop);
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();

}

GameScene::~GameScene()
{
    delete m_tilemap;
    delete m_keymap;
    delete m_db;
    delete m_player;
    delete m_UI;
    clear();
}

void GameScene::loop()
{
    m_deltaTime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_player->input(m_keymap);
    m_player->update(m_deltaTime);
    updateCamera();
    m_tilemap->setPos(-m_cameraPosX, -m_cameraPosY);
    m_tilemap->input(m_keymap);
    setBackgroundBrush(QBrush(m_tilemap->bgColour()));
    //setSceneRect(m_cameraPosX, m_cameraPosY, sceneRect().width(), sceneRect().height());
    //m_tilemap->update(m_deltaTime);
    /*
    // Input and update GameLayer according to flags
    switch (m_currentLayer)
    {
    case WORLD:
        if (m_player->enteredCombat())
        {
            m_keymap->setTurnBasedBindings();
            m_turnbased->setBackground(QPixmap(":/image/background/res/battleBackground.png").scaled(sceneRect().width(), sceneRect().height()));
            m_currentLayer = TURN_BASED;
            m_turnbased->setEvent(m_player, m_tilemap->getCombatSprites());

            m_tilemap->removeItem(*this);
            m_player->removeItem(*this);
            m_UI->removeItem(*this);
            m_turnbased->render(*this);
            //m_bufferImage = QImage(sceneRect().toAlignedRect().size(), QImage::Format_ARGB32);

        }
        break;
    case TURN_BASED:
        break;
    case BULLET_HELL:
        break;
    case MENU:
        break;
    case MAIN_MENU:
        break;
    case PAUSE_MENU:
        break;
    }

    // Update and render depending on layer
    switch (m_currentLayer)
    {
    case WORLD:
        // INPUT
        m_player->input(m_keymap);
        m_UI->input(m_keymap, m_player->activeCharacter());

        // UPDATE
        updateCamera();
        m_tilemap->update(m_deltaTime);
        m_player->update(m_deltaTime);
        m_UI->update(m_deltaTime);

        // RENDER
        m_tilemap->render(*m_buffer);

        // Remove all items
        m_tilemap->removeItem(*this);
        m_player->removeItem(*this);
        m_UI->removeItem(*this);
        // Render all items;
        setSceneRect(m_cameraPosX, m_cameraPosY, sceneRect().width(), sceneRect().height());
        m_tilemap->render(*this);
        m_player->render(*this);
        m_UI->render(*this);
        break;
    case TURN_BASED:
        // INPUT
        m_turnbased->input(m_keymap);

        // UPDATE
        m_turnbased->update(m_deltaTime);

        // RENDER
        /*
        // Remove all items
        m_turnbased->removeItem(*this);
        // Render all
        m_turnbased->render(*this);

        break;
    case BULLET_HELL:
        break;
    case MENU:
        break;
    case MAIN_MENU:
        break;
    case PAUSE_MENU:
        break;
    }

    // Change buffer
    m_buffer->setPos(-m_cameraPosX, -m_cameraPosY);
    m_buffer->swap();
    */
    // Reset mouse and key status

    m_keymap->resetStatus();
}

void GameScene::updateCamera()
{
    m_cameraPosX = m_player->getSprite()->x() +
                   m_player->getSprite()->boundingRect().width()/2 -
                   sceneRect().width()/2;
    m_cameraPosY = m_player->getSprite()->y() +
                   m_player->getSprite()->boundingRect().height()/2 -
                   sceneRect().height()/2;
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
    if (!event->isAutoRepeat())
    {
        Qt::Key key = static_cast<Qt::Key>(event->key());

        if (m_keymap->contains(key))
        {
            GLOBAL::Action action = m_keymap->getAction(key);
            m_keymap->keyReleased(action);
        }
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
