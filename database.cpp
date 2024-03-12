#include "database.h"

#include <QDebug>

#include "movingsprite.h"

Database::Database()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("GameDatabase.db");

    if (!m_db.open()) {
        qDebug() << "Error: database failed to open";
    }
    else {
        qDebug() << "Database opened";
    }
}

Database::~Database()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool Database::isOpen() const
{
    return m_db.isOpen();
}

QSqlQuery Database::getMapInfo(int MapID)
{
    QSqlQuery query(QString("SELECT * FROM Map WHERE MapID = %1").arg(MapID));
    return query;
}

QList<Tile*> Database::getMapTiles(int MapID)
{
    QSqlQuery query(QString("SELECT TileInMapIndex, Tile.TileID, TileName, TileDescription, PathToTileSheet, NumberOfTiles "
                            "FROM TileInMap, Tile "
                            "WHERE (Tile.TileID = TileInMap.TileID) AND (MapID = %1) "
                            "ORDER BY TileInMapIndex ASC").arg(MapID));
    QString filePath;
    QPixmap image;
    int NumberOfTiles;
    QList<Tile*> tiles;

    while (query.next())
    {
        filePath = query.value("PathToTileSheet").toString();
        NumberOfTiles = query.value("NumberOfTiles").toInt();
        image.load(filePath);

        for (int n = 0; n < NumberOfTiles; n++)
        {
            tiles.append(new Tile);
            tiles.back()->setPixmap(image.scaled(GLOBAL::ObjectSize));
            tiles.back()->setID(query.value("Tile.TileID").toInt());
            tiles.back()->setDescription(query.value("TileDescription").toString());
            tiles.back()->setName(query.value("TileName").toString());
        }
    }

    return tiles;
}

QList<Sprite*> Database::getWorldSprites(int MapID)
{
    QSqlQuery query(QString("SELECT Sprite.SpriteID, SpriteName, SpriteType, SpriteSizeX, SpriteSizeY, PathToSpriteSheet, Interactable, InteractText, InteractDialogue, PositionX, PositionY "
                          "FROM SpriteInMap, Sprite "
                          "WHERE (Sprite.SpriteID = SpriteInMap.SpriteID) AND (MapID = %1)  AND (SpriteType = 'WorldSprite') "
                            "ORDER BY PositionX ASC, PositionY ASC").arg(MapID));
    QSqlQuery animationQuery;
    QPixmap image;

    QList<Sprite*> sprites;
    while(query.next())
    {
        sprites.append(new Sprite);
        sprites.back()->setID(query.value("Sprite.SpriteID").toInt());
        sprites.back()->setName(query.value("SpriteName").toString());
        sprites.back()->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                                             query.value("SpriteSizeY").toInt()));

        image.load(query.value("PathToSpriteSheet").toString());
        sprites.back()->setSpriteSheet(image);

        if (query.value("Interactable").toBool())
        {
            sprites.back()->setInteraction(query.value("InteractText").toString(),
                                             query.value("InteractDialogue").toString());
        }
        sprites.back()->setPos(query.value("PositionX").toInt()*GLOBAL::ObjectLength,
                                 query.value("PositionY").toInt()*GLOBAL::ObjectLength);
        sprites.back()->setZValue(GLOBAL::SPRITE_LAYER);

        animationQuery = getSpriteAnimations(sprites.back()->getID());
        while (animationQuery.next()) {
            sprites.back()->addAnimationState(animationQuery.value("StateName").toString(),
                                                animationQuery.value("StartFrame").toInt(),
                                                animationQuery.value("EndFrame").toInt(),
                                                animationQuery.value("FrameTime").toFloat());
        }
    }

    return sprites;
}

QList<MovingSprite*> Database::getMovingSpritesFromMap(int MapID)
{
    QSqlQuery query(QString("SELECT Sprite.SpriteID, SpriteName, SpriteType, SpriteSizeX, SpriteSizeY, PathToSpriteSheet, Interactable, InteractText, InteractDialogue, PositionX, PositionY "
                            "FROM SpriteInMap, Sprite "
                            "WHERE (Sprite.SpriteID = SpriteInMap.SpriteID) AND (MapID = %1) AND (SpriteType = 'MovingSprite') "
                            "ORDER BY PositionX ASC, PositionY ASC").arg(MapID));
    QSqlQuery animationQuery;
    QPixmap image;

    QList<MovingSprite*> sprites;
    while(query.next())
    {
        sprites.append(new MovingSprite);
        sprites.back()->setID(query.value("Sprite.SpriteID").toInt());
        sprites.back()->setName(query.value("SpriteName").toString());
        sprites.back()->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                                           query.value("SpriteSizeY").toInt()));

        image.load(query.value("PathToSpriteSheet").toString());
        sprites.back()->setSpriteSheet(image);

        if (query.value("Interactable").toBool())
        {
            sprites.back()->setInteraction(query.value("InteractText").toString(),
                                           query.value("InteractDialogue").toString());
        }
        sprites.back()->setPos(query.value("PositionX").toInt()*GLOBAL::ObjectLength,
                               query.value("PositionY").toInt()*GLOBAL::ObjectLength);
        sprites.back()->setZValue(GLOBAL::SPRITE_LAYER);

        animationQuery = getSpriteAnimations(sprites.back()->getID());
        while (animationQuery.next()) {
            sprites.back()->addAnimationState(animationQuery.value("StateName").toString(),
                                              animationQuery.value("StartFrame").toInt(),
                                              animationQuery.value("EndFrame").toInt(),
                                              animationQuery.value("FrameTime").toFloat());
        }
    }

    return sprites;
}

MovingSprite* Database::getMovingSprite(int SpriteID)
{
    QSqlQuery query(QString("SELECT * FROM Sprite "
                            "WHERE (SpriteID = %1)").arg(SpriteID));

    QSqlQuery animationQuery;
    QPixmap image;
    MovingSprite* sprite = new MovingSprite;

    query.next();
    sprite->setID(query.value("Sprite.SpriteID").toInt());
    sprite->setName(query.value("SpriteName").toString());
    sprite->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                                       query.value("SpriteSizeY").toInt()));

    image.load(query.value("PathToSpriteSheet").toString());
    sprite->setSpriteSheet(image);

    animationQuery = getSpriteAnimations(sprite->getID());
    while (animationQuery.next()) {
        sprite->addAnimationState(animationQuery.value("StateName").toString(),
                                  animationQuery.value("StartFrame").toInt(),
                                  animationQuery.value("EndFrame").toInt(),
                                  animationQuery.value("FrameTime").toFloat());
    }

    return sprite;
}

QSqlQuery Database::getSpriteAnimations(int SpriteID)
{
    QSqlQuery query(QString("SELECT StateName, StartFrame, EndFrame, FrameTime "
                            "FROM SpriteAnimations "
                            "WHERE (SpriteID = %1)").arg(SpriteID));
    return query;
}
