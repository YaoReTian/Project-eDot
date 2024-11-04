#include "gamescene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include "Entities/bullet.h"

#include "Utils/global.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent), m_tilemap(new Tilemap), m_db(new Database),
    m_player(new Player(m_tilemap)), m_bulletManager(m_db->getBulletManager()),
    m_buttonManager(new ButtonManager), m_cameraPosX(0), m_cameraPosY(0),
    m_gameOver(new Menu), m_start(new Menu), m_pause(new Menu), m_loadGame(new Menu), m_saveGame(new Menu)
{
    // Setting up tilemap
    m_tilemap->setDatabase(m_db);
    m_tilemap->setBulletManager(m_bulletManager);
    m_tilemap->hide();
    addItem(m_tilemap);

    TileSet ts(":/tileset/res/General.tsj",1);
    TileInfo* sprite = ts.getInfo(1);
    m_db->setSpriteData(sprite->m_path, m_player);
    for (const auto h : std::as_const(sprite->m_hitboxes))
    {
        m_player->setHitbox(h);
    }
    m_player->setBulletManager(m_bulletManager);

    m_tilemap->setZValue(0);

    // Creating menus

    // Game over
    QColor c(Qt::red);
    c.setAlphaF(0.5);
    m_gameOver->setBrush(c);
    m_gameOver->setZValue(1);
    m_gameOver->setText(QPixmap(":/image/res/Game-over.png"));
    addItem(m_gameOver);

    // Start
    m_start->setBrush(Qt::black);
    m_start->setZValue(1);
    m_start->setMessage("PRESS TAB TO CHANGE OPTIONS");
    m_start->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_start);
    m_start->show();

    // Pause
    QColor x(Qt::gray);
    x.setAlphaF(0.5);
    m_pause->setBrush(x);
    m_pause->setZValue(1);
    m_pause->setMessage("GAME PAUSED");
    m_pause->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_pause);

    // Save game
    m_saveGame->setBrush(x);
    m_saveGame->setZValue(1);
    m_saveGame->setMessage("SAVE GAME - PRESS ESC TO RETURN");
    m_saveGame->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_saveGame);

    // Load game
    m_loadGame->setBrush(Qt::black);
    m_loadGame->setZValue(2);
    m_loadGame->setMessage("SAVE SLOTS - PRESS ESC TO RETURN");
    m_loadGame->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_loadGame);

    start();

    connect(m_player, SIGNAL(dead()), this, SLOT(gameOver()));
}

GameScene::~GameScene()
{
    delete m_db;
    delete m_bulletManager;
    clear();
}

void GameScene::input(KeyMap* keys)
{
    m_buttonManager->input(keys);
    if (m_start->isVisible())
    {
        m_start->setRect(0,0, width(), height());
        m_buttonManager->setPos(m_start->boundingRect().width()/2 - m_buttonManager->width()/2,
                                m_start->boundingRect().height()/2 + 12*GLOBAL::Scale);
    }
    else if (m_player->HP() == 0)
    {
        m_gameOver->setRect(0,0, width(), height());
        m_buttonManager->setPos(m_gameOver->boundingRect().width()/2 - m_buttonManager->width()/2,
                                m_gameOver->boundingRect().height()/2 + 12*GLOBAL::Scale);
    }
    else if (m_pause->isVisible())
    {
        if (keys->keyReleasedStatus(GLOBAL::PAUSE))
        {
            unpause();
        }
        else
        {
            m_pause->setRect(0,0, width(), height());
            m_buttonManager->setPos(m_pause->boundingRect().width()/2 - m_buttonManager->width()/2,
                                    m_pause->boundingRect().height()/2 + 12*GLOBAL::Scale);
        }
    }
    else if (m_loadGame->isVisible())
    {
        if (keys->keyReleasedStatus(GLOBAL::PAUSE))
        {
            m_loadGame->hide();
            start();
        }
        else
        {
            m_loadGame->setRect(0,0, width(), height());
            m_buttonManager->setPos(m_loadGame->boundingRect().width()/2 - m_buttonManager->width()/2,
                                    m_loadGame->boundingRect().height()/2 + 12*GLOBAL::Scale);
        }
    }
    else if (m_saveGame->isVisible())
    {
        if (keys->keyReleasedStatus(GLOBAL::PAUSE))
        {
            m_saveGame->hide();
            pause();
        }
        else
        {
            m_saveGame->setRect(0,0, width(), height());
            m_buttonManager->setPos(m_saveGame->boundingRect().width()/2 - m_buttonManager->width()/2,
                                    m_saveGame->boundingRect().height()/2 + 12*GLOBAL::Scale);
        }
    }
    else
    {
        if (keys->keyReleasedStatus(GLOBAL::PAUSE))
        {
            if (!m_pause->isVisible())
            {
                pause();
            }
            else
            {
                unpause();
            }
        }
        keys->setMousePos(keys->mousePos().x() + m_cameraPosX, keys->mousePos().y() + m_cameraPosY);
        m_player->input(keys);
        m_tilemap->input(keys, m_player->pos());
    }
}

void GameScene::update(int deltatime)
{
    m_buttonManager->update(deltatime);
    if (m_start->isVisible())
    {
        m_start->update(deltatime);
    }
    else if (m_player->HP() == 0)
    {
        m_gameOver->update(deltatime);
    }
    else if (m_pause->isVisible())
    {
        m_pause->update(deltatime);
    }
    else if (m_loadGame->isVisible())
    {
        m_loadGame->update(deltatime);
    }
    else if (m_saveGame->isVisible())
    {
        m_saveGame->update(deltatime);
    }
    else
    {
        m_player->update(deltatime);
        m_bulletManager->update(deltatime);
        updateCamera();
        m_tilemap->setPos(-m_cameraPosX, -m_cameraPosY);
        m_tilemap->update(deltatime);
    }
}

void GameScene::updateCamera()
{
    m_cameraPosX = m_player->x() +
                   m_player->boundingRect().width()/2 -
                   sceneRect().width()/2;
    m_cameraPosY = m_player->y() +
                   m_player->boundingRect().height()/2 -
                   sceneRect().height()/2;
}

// SLOTS

void GameScene::start()
{
    m_buttonManager->setParentItem(m_start);
    m_buttonManager->clear();
    m_start->show();
    Button* b[3];
    QString t[3] = {"NEW GAME", "LOAD GAME", "QUIT"};
    const char *method[3] = {SLOT(newGame()), SLOT(saveSlots()), SLOT(quit())};
    for (int i= 0; i < 3; i++)
    {
        b[i] = m_buttonManager->getButtonFromPool();
        b[i]->setText(t[i]);
        connect(b[i], SIGNAL(triggered(int)), this, method[i]);
        m_buttonManager->addButton(b[i]);
    }
}

void GameScene::newGame()
{
    m_tilemap->setMap(3);
    m_player->setZValue(m_tilemap->getPlayerZ());
    m_player->setPos(5*GLOBAL::ObjectLength,5*GLOBAL::ObjectLength);
    setBackgroundBrush(QBrush(m_tilemap->bgColour()));
    m_start->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_tilemap);
    m_tilemap->show();
}

void GameScene::saveSlots()
{
    m_start->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_loadGame);
    m_loadGame->show();
    Button* b[5];
    SaveSlot* slot;
    for (int i= 0; i < 5; i++)
    {
        slot = m_db->getSaveSlot(i+1);
        b[i] = m_buttonManager->getButtonFromPool();
        b[i]->setText("Slot " + QString::number(i+1) + ": " + slot->m_username);
        b[i]->addTriggerValue(i+1);
        if (!slot->m_username.isEmpty())
        {
            connect(b[i], SIGNAL(triggered(int)), this, SLOT(loadGame(int)));
        }
        m_buttonManager->addButton(b[i]);
    }
}

void GameScene::loadGame(int slotID)
{
    SaveSlot* s = m_db->getSaveSlot(slotID);
    m_tilemap->setMap(s->m_mapID);
    m_player->setPos(s->m_tilePosX*GLOBAL::ObjectLength, s->m_tilePosY*GLOBAL::ObjectLength);
    m_player->setZValue(m_tilemap->getPlayerZ());
    setBackgroundBrush(QBrush(m_tilemap->bgColour()));
    m_loadGame->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_tilemap);
    m_tilemap->show();
}

void GameScene::gameOver()
{
    m_gameOver->show();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_gameOver);
    Button* b1 = m_buttonManager->getButtonFromPool();
    Button* b2 = m_buttonManager->getButtonFromPool();
    b1->setText("RESTART");
    b2->setText("END GAME");

    connect(b1, SIGNAL(triggered(int)), this, SLOT(restartFromCheckpoint()));
    connect(b2, SIGNAL(triggered(int)), this, SLOT(endGame()));

    m_buttonManager->addButton(b1);
    m_buttonManager->addButton(b2);
}

void GameScene::endGame()
{
    m_gameOver->hide();
    m_pause->hide();
    m_tilemap->clear();
    m_player->reset();
    m_tilemap->hide();
    m_bulletManager->clear();
    start();
}

void GameScene::quit()
{
    QApplication::quit();
}

void GameScene::restartFromCheckpoint()
{
    m_gameOver->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_tilemap);
    m_tilemap->reset();
    m_player->reset();
}

void GameScene::pause()
{
    m_pause->show();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_pause);
    Button* b[3];
    QString t[3] = {"CONTINUE", "SAVE", "END GAME"};
    const char *method[3] = {SLOT(unpause()), SLOT(saveMenu()),SLOT(endGame())};

    for (int i= 0; i < 3; i++)
    {
        b[i] = m_buttonManager->getButtonFromPool();
        b[i]->setText(t[i]);
        connect(b[i], SIGNAL(triggered(int)), this, method[i]);
        m_buttonManager->addButton(b[i]);
    }
}

void GameScene::unpause()
{
    m_pause->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_tilemap);
}

void GameScene::saveMenu()
{
    m_pause->hide();
    m_saveGame->show();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_saveGame);
    Button* b[5];
    SaveSlot* slot;
    for (int i= 0; i < 5; i++)
    {
        slot = m_db->getSaveSlot(i+1);
        b[i] = m_buttonManager->getButtonFromPool();
        b[i]->setText("Slot " + QString::number(i+1) + ": " + slot->m_username);
        b[i]->addTriggerValue(i+1);
        if (!slot->m_username.isEmpty())
        {
            connect(b[i], SIGNAL(triggered(int)), this, SLOT(save(int)));
        }
        m_buttonManager->addButton(b[i]);
    }
}

void GameScene::save(int slotID)
{
    m_db->saveGame(slotID,
                   m_player->username(),
                   m_player->x()/GLOBAL::ObjectLength,
                   m_player->y()/GLOBAL::ObjectLength,
                   m_tilemap->mapID());
    saveMenu();
}
