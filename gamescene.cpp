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
    m_gameOver(new Menu), m_start(new Menu), m_pause(new Menu), m_loadGame(new Menu),
    m_saveGame(new Menu), m_messageBox(new Message), m_inputBox(new InputBox)
{
    // Setting up tilemap
    m_tilemap->setDatabase(m_db);
    m_tilemap->setBulletManager(m_bulletManager);
    m_tilemap->hide();
    addItem(m_tilemap);

    TileSet ts(":/tileset/res/General.tsj",1);
    TileInfo* sprite = ts.getInfo(2);
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
    m_pause->setZValue(3);
    m_pause->setMessage("GAME PAUSED");
    m_pause->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_pause);

    // Save game
    m_saveGame->setBrush(x);
    m_saveGame->setZValue(3);
    m_saveGame->setMessage("SAVE GAME - PRESS ESC TO RETURN");
    m_saveGame->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_saveGame);

    // Load game
    m_loadGame->setBrush(Qt::black);
    m_loadGame->setZValue(1);
    m_loadGame->setMessage("SAVE SLOTS - PRESS ESC TO RETURN");
    m_loadGame->setText(QPixmap(":/image/res/Game-logo.png"));
    addItem(m_loadGame);

    start();

    m_bulletManager->setPlayer(m_player);
    connect(m_player, SIGNAL(dead()), this, SLOT(gameOver()));
    connect(m_player, SIGNAL(collidedWithLink(int)), this, SLOT(nextMapButton(int)));

    m_messageBox->setZValue(2);
    addItem(m_messageBox);
    m_inputBox->setZValue(2);
    addItem(m_inputBox);
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
    if (m_inputBox->isVisible())
    {
        m_inputBox->input(keys);
        m_inputBox->setPos(width()/2 - m_inputBox->boundingRect().width()/2,
                           height()/2 - m_inputBox->boundingRect().height()/2);
    }
    if (m_messageBox->isVisible())
    {
        m_messageBox->input(keys);
        m_messageBox->setPos(width()/2 - m_messageBox->boundingRect().width()/2,
                             height()-m_messageBox->boundingRect().height() - 2*GLOBAL::Scale);
    }
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
        m_buttonManager->setPos(m_player->boundingRect().width() + 2*GLOBAL::Scale,0);
    }
}

void GameScene::update(int deltatime)
{
    m_buttonManager->update(deltatime);
    if (m_inputBox->isVisible())
    {
        m_inputBox->update(deltatime);
    }
    if (m_messageBox->isVisible())
    {
        m_messageBox->update(deltatime);
    }
    if (m_start->isVisible())
    {
        m_start->update(deltatime);
    }
    else if (m_gameOver->isVisible())
    {
        m_gameOver->update(deltatime);
    }
    else if (m_pause->isVisible())
    {
        m_pause->update(deltatime);
        updateCamera();
        m_tilemap->setPos(-m_cameraPosX, -m_cameraPosY);
    }
    else if (m_loadGame->isVisible())
    {
        m_loadGame->update(deltatime);
    }
    else if (m_saveGame->isVisible())
    {
        m_saveGame->update(deltatime);
        updateCamera();
        m_tilemap->setPos(-m_cameraPosX, -m_cameraPosY);
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
    m_player->setPos(m_tilemap->playerStartPos());
    setBackgroundBrush(QBrush(m_tilemap->bgColour()));
    m_start->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_player);
    m_tilemap->show();
    m_player->setCharge(0);
    m_player->freeze();

    m_messageBox->setDialogue("You are a mercenary hired to kill 'Bot Boy'");
    m_messageBox->setDialogue("What is your name?");
    m_messageBox->setFinishCondition(Message::signal);
    m_messageBox->show();

    connect(m_messageBox, SIGNAL(complete()), m_inputBox, SLOT(show()));
    connect(m_inputBox, SIGNAL(textInputted(QString)), m_messageBox, SLOT(receiveSignal()));
    connect(m_inputBox, SIGNAL(textInputted(QString)), this, SLOT(intro(QString)));
}

void GameScene::intro(QString username)
{
    m_inputBox->hide();
    m_inputBox->disconnect();
    m_messageBox->disconnect();
    m_player->setUsername(username);
    m_messageBox->setDialogue("'Bot Boy' is at the top of this tower.");
    m_messageBox->setDialogue("Get to the top of the tower and take it back!");
    m_messageBox->setDialogue("Do you want to play the tutorial?");
    m_messageBox->setFinishCondition(Message::signal);
    m_messageBox->show();

    Button *b1 = m_buttonManager->getButtonFromPool();
    Button *b2 = m_buttonManager->getButtonFromPool();
    b1->setText("Yes");
    b2->setText("No");
    m_buttonManager->addButton(b1);
    m_buttonManager->addButton(b2);

    m_buttonManager->hide();
    connect(m_messageBox, SIGNAL(complete()), m_buttonManager, SLOT(show()));
    connect(b1, SIGNAL(triggered(int)), m_messageBox, SLOT(receiveSignal()));
    connect(b2, SIGNAL(triggered(int)), m_messageBox, SLOT(receiveSignal()));
    connect(b1, SIGNAL(triggered(int)), this, SLOT(movementTutorial()));
    connect(b2, SIGNAL(triggered(int)), this, SLOT(finishTutorial()));
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
    m_player->heal(s->m_HP);
    m_player->setCharge(s->m_charge);
    setBackgroundBrush(QBrush(m_tilemap->bgColour()));
    m_loadGame->hide();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_player);
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
    m_player->disconnect();
    m_messageBox->disconnect();
    m_nextMapButtonVisible = false;
    m_player->reset();
    m_messageBox->hide();
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
        connect(b[i], SIGNAL(triggered(int)), this, SLOT(save(int)));
        m_buttonManager->addButton(b[i]);
    }
}

void GameScene::save(int slotID)
{
    m_db->saveGame(slotID,
                   m_player->username(),
                   m_player->x()/GLOBAL::ObjectLength,
                   m_player->y()/GLOBAL::ObjectLength,
                   m_tilemap->mapID(),
                   m_player->HP(),
                   m_player->charge());
    saveMenu();
}

void GameScene::movementTutorial()
{
    m_player->setTutorialMode();
    m_buttonManager->clear();

    m_messageBox->setName("Tutorial");
    m_messageBox->setDialogue("Use WASD keys for movement.");
    m_messageBox->setFinishCondition(Message::signal);
    m_messageBox->show();

    connect(m_messageBox, SIGNAL(complete()), m_player, SLOT(unfreeze()));
    connect(m_messageBox, SIGNAL(hiding()), this, SLOT(slowTutorial()));
    connect(m_player, SIGNAL(moved()), m_messageBox, SLOT(receiveSignal()));
}

void GameScene::slowTutorial()
{
    m_player->disconnect();
    m_messageBox->disconnect();
    m_messageBox->setName("Tutorial");
    m_messageBox->setDialogue("Click SHIFT to slow down and show your hitbox.");
    m_messageBox->setFinishCondition(Message::signal);
    m_messageBox->show();

    connect(m_messageBox, SIGNAL(complete()), m_player, SLOT(nextTutorial()));
    connect(m_messageBox, SIGNAL(hiding()), this, SLOT(shootTutorial()));
    connect(m_player, SIGNAL(slowMoved()), m_messageBox, SLOT(receiveSignal()));
}

void GameScene::shootTutorial()
{
    m_player->disconnect();
    m_messageBox->disconnect();
    m_messageBox->setName("Tutorial");
    m_messageBox->setDialogue("Use your mouse to aim and click SPACE to shoot.");
    m_messageBox->setFinishCondition(Message::signal);
    m_messageBox->show();

    connect(m_messageBox, SIGNAL(complete()), m_player, SLOT(nextTutorial()));
    connect(m_player, SIGNAL(shooting()), m_messageBox, SLOT(receiveSignal()));
    connect(m_messageBox, SIGNAL(hiding()), this, SLOT(overdriveTutorial()));
}

void GameScene::overdriveTutorial()
{
    m_player->disconnect();
    m_messageBox->disconnect();
    m_player->setCharge(50);
    m_messageBox->setName("Tutorial");
    m_messageBox->setDialogue("'Quantum Overdrive' will be charged when you graze a bullet.");
    m_messageBox->setDialogue("For this tutorial, it will be charged now.");
    m_messageBox->setDialogue("Your 'Quantum Overdrive' is now charged, click Q to use it.");
    m_messageBox->setFinishCondition(Message::signal);
    m_messageBox->show();

    connect(m_messageBox, SIGNAL(complete()), m_player, SLOT(nextTutorial()));
    connect(m_player, SIGNAL(overdriveUsed()), m_messageBox, SLOT(receiveSignal()));
    connect(m_messageBox, SIGNAL(hiding()), this, SLOT(finishTutorial()));
}

void GameScene::finishTutorial()
{
    m_buttonManager->clear();
    m_player->unfreeze();
    m_messageBox->disconnect();
    m_player->disconnect();
    m_bulletManager->clear();
    m_player->setTutorialMode(false);
    connect(m_player, SIGNAL(dead()), SLOT(gameOver()));
    connect(m_player, SIGNAL(collidedWithLink(int)), this, SLOT(nextMapButton(int)));
}

void GameScene::message(QString dialogue, Message::FinishCondition condition, QString name)
{
    if (condition == Message::keyPress)
    {
        m_player->freeze();
    }

    if (!name.isEmpty())
    {
        m_messageBox->setName(name);
    }

    m_messageBox->setDialogue(dialogue);
    m_messageBox->setFinishCondition(condition);
    m_messageBox->show();

    connect(m_messageBox, SIGNAL(complete()), m_player, SLOT(unfreeze()));
}

void GameScene::nextMapButton(int mapID)
{
    if (m_buttonManager->numActiveButtons() == 0 && m_tilemap->enemiesCleared())
    {
        Button* b = m_buttonManager->getButtonFromPool();
        b->setText("Go to next map.");
        b->addTriggerValue(mapID);
        connect(b,SIGNAL(triggered(int)), this, SLOT(changeMap(int)));
        m_buttonManager->addButton(b);
    }
}

void GameScene::closeMapButton()
{
    m_buttonManager->clear();
    m_nextMapButtonVisible = false;
}

void GameScene::changeMap(int mapID)
{
    m_bulletManager->clear();
    m_buttonManager->clear();
    m_tilemap->setMap(mapID);
    m_player->setPos(m_tilemap->playerStartPos());
}
