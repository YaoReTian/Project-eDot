#include "gamescene.h"

#include <QDebug>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QImage>

#include "Utils/global.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent), m_tilemap(new Tilemap), m_db(new Database),
    m_player(new Player(m_tilemap)), m_cameraPosX(0), m_cameraPosY(0)
{
    m_tilemap->setDatabase(m_db);
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
    m_player->setPos(4*GLOBAL::ObjectLength,4*GLOBAL::ObjectLength);
}

GameScene::~GameScene()
{
    delete m_db;
    clear();
}

void GameScene::input(KeyMap* keys)
{
    m_player->input(keys);
    m_tilemap->input(keys);
}

void GameScene::update(int deltatime)
{
    m_player->update(deltatime);
    updateCamera();
    m_tilemap->setPos(-m_cameraPosX, -m_cameraPosY);
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
