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
    m_gameOver(new GameOver)
{
    m_tilemap->setDatabase(m_db);
    m_tilemap->setBulletManager(m_bulletManager);
    m_tilemap->setMap(3);
    addItem(m_tilemap);
    setBackgroundBrush(QBrush(m_tilemap->bgColour()));

    TileSet ts(":/tileset/res/General.tsj",1);
    TileInfo* sprite = ts.getInfo(1);
    m_db->setSpriteData(sprite->m_path, m_player);
    for (const auto h : std::as_const(sprite->m_hitboxes))
    {
        m_player->setHitbox(h);
    }
    m_player->setZValue(m_tilemap->getPlayerZ());
    m_player->setPos(20*GLOBAL::ObjectLength,8*GLOBAL::ObjectLength);
    m_player->setBulletManager(m_bulletManager);

    m_tilemap->setZValue(0);
    m_gameOver->setZValue(1);
    addItem(m_gameOver);

    m_buttonManager->setParentItem(m_tilemap);
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
    if (m_player->HP() == 0)
    {
        m_gameOver->setRect(0,0, width(), height());
        m_buttonManager->setPos(m_gameOver->boundingRect().width()/2 - m_buttonManager->width()/2,
                                m_gameOver->boundingRect().height()/2 + 10*GLOBAL::Scale);
    }
    else
    {
        keys->setMousePos(keys->mousePos().x() + m_cameraPosX, keys->mousePos().y() + m_cameraPosY);
        m_player->input(keys);
        m_tilemap->input(keys, m_player->pos());
    }
}

void GameScene::update(int deltatime)
{
    m_buttonManager->update(deltatime);
    if (m_player->HP() == 0)
    {
        m_gameOver->update(deltatime);
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

void GameScene::gameOver()
{
    m_gameOver->show();
    m_buttonManager->clear();
    m_buttonManager->setParentItem(m_gameOver);
    Button* b1 = m_buttonManager->getButtonFromPool();
    Button* b2 = m_buttonManager->getButtonFromPool();
    b1->setText("RESTART");
    b2->setText("END GAME");

    connect(b1, SIGNAL(triggered()), this, SLOT(restartFromCheckpoint()));
    connect(b2, SIGNAL(triggered()), this, SLOT(endGame()));

    m_buttonManager->addButton(b1);
    m_buttonManager->addButton(b2);
}

void GameScene::endGame()
{
    QApplication::quit();
}

void GameScene::restartFromCheckpoint()
{
    m_gameOver->hide();
    m_buttonManager->clear();
    m_tilemap->reset();
    m_player->reset();
}
