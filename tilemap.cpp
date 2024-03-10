#include "tilemap.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>

#include "global.h"

// Public methods
Tilemap::Tilemap(){}

Tilemap::~Tilemap()
{
    qDebug() << "Tilemap destroyed";
}

void Tilemap::setMap(int MapID)
{
    QSqlQuery query;

    if (m_db.isOpen()) {
        query = m_db.getMapInfo(MapID);
    } else {
        qDebug() << "Database is not open";
    }

    query.next();
    m_mapID = query.value("MapID").toInt();
    m_mapName = query.value("MapName").toString();
    m_mapDesc = query.value("MapDescription").toString();
    m_mapSizeX = query.value("MapSizeX").toInt();
    m_mapSizeY = query.value("MapSizeY").toInt();

    setTiles();
    setSprites();
}

QString Tilemap::getMapDesc()
{
    return m_mapDesc;
}

QString Tilemap::getMapName()
{
    return m_mapName;
}

void Tilemap::update(int deltatime)
{
    for (const auto &s : m_sprites)
    {
        s->update(deltatime);
    }
}

void Tilemap::setTiles()
{
    QSqlQuery query;
    QString filePath;
    QPixmap image;
    int NumberOfTiles;

    if (m_db.isOpen())
    {
        query = m_db.getMapTiles(m_mapID);
    }
    else
    {
        qDebug() << "Database is not open";
    }

    while (query.next())
    {
        filePath = query.value("PathToTileSheet").toString();
        NumberOfTiles = query.value("NumberOfTiles").toInt();
        image.load(filePath);

        for (int n = 0; n < NumberOfTiles; n++)
        {
            m_tiles.append(new Tile);
            m_tiles.back()->setPixmap(image.scaled(GLOBAL::ObjectSize));
            m_tiles.back()->setID(query.value("Tile.TileID").toInt());
            m_tiles.back()->setDescription(query.value("TileDescription").toString());
            m_tiles.back()->setName(query.value("TileName").toString());
        }
    }

    if (m_mapSizeX * m_mapSizeY != m_tiles.size()) {
        qDebug() << m_mapSizeX * m_mapSizeY;
        qDebug() << m_tiles.size();
        qDebug() << "ERROR: NUMBER OF TILES != AREA";
    }
}

void Tilemap::setSprites()
{
    QSqlQuery query;
    QSqlQuery animationQuery;
    QPixmap image;

    if (m_db.isOpen())
    {
        query = m_db.getMapSprites(m_mapID);
    }
    else
    {
        qDebug() << "Database is not open";
    }

    if (!query.isActive())
    {
        qDebug() << "Error: query is not active" << query.lastError();
    }

    while(query.next())
    {
        m_sprites.append(new Sprite);
        m_sprites.back()->setID(query.value("Sprite.SpriteID").toInt());
        m_sprites.back()->setName(query.value("SpriteName").toString());
        m_sprites.back()->setType(query.value("SpriteType").toString());
        m_sprites.back()->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                                             query.value("SpriteSizeY").toInt()));

        image.load(query.value("PathToSpriteSheet").toString());
        m_sprites.back()->setSpriteSheet(image);

        if (query.value("Interactable").toBool())
        {
            m_sprites.back()->setInteraction(query.value("InteractText").toString(),
                                             query.value("InteractDialogue").toString());
        }
        m_sprites.back()->setPos(query.value("PositionX").toInt()*GLOBAL::ObjectLength,
                                 query.value("PositionY").toInt()*GLOBAL::ObjectLength);
        m_sprites.back()->setZValue(GLOBAL::SPRITE_LAYER);

        animationQuery = m_db.getSpriteAnimations(m_sprites.back()->getID());
        while (animationQuery.next()) {
            m_sprites.back()->addAnimationState(animationQuery.value("StateName").toString(),
                                                animationQuery.value("StartFrame").toInt(),
                                                animationQuery.value("EndFrame").toInt(),
                                                animationQuery.value("FrameTime").toFloat());
        }
    }


}

void Tilemap::generateTiles(QGraphicsScene &scene)
{
    int tileIndex = 0;

    for (int y = 0; y < GLOBAL::ObjectLength*m_mapSizeY; y += GLOBAL::ObjectLength)
    {
        for (int x = 0; x < GLOBAL::ObjectLength*m_mapSizeX; x += GLOBAL::ObjectLength)
        {
            m_tiles[tileIndex]->setPos(x,y);
            m_tiles[tileIndex]->setZValue(GLOBAL::TILE_LAYER);
            scene.addItem(m_tiles[tileIndex]);
            tileIndex++;
        }
    }
}

void Tilemap::generateSprites(QGraphicsScene &scene)
{
    for (const auto &n : m_sprites)
    {
        n->update(0);
        scene.addItem(n);
    }
}

void Tilemap::updateTiles()
{

}

void Tilemap::updateSprites()
{

}
