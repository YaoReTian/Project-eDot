#include "database.h"

#include <QSqlQuery>
#include <QDir>
#include <QDebug>
#include <QSqlRecord>

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

QString Database::getTestTilePath()
{
    QSqlQuery query("SELECT * FROM Tile WHERE TileID = 1");
    int pathIndex = query.record().indexOf("PathToTileSheet");
    query.next();
    QString filePath = query.value(pathIndex).toString();
    return filePath;
}

QSqlQuery Database::getMapInfo(int MapID)
{
    QSqlQuery query(QString("SELECT * FROM Map WHERE MapID = %1").arg(MapID));
    return query;
}

QSqlQuery Database::getMapTiles(int MapID)
{
    QSqlQuery query(QString("SELECT TileInMapIndex, Tile.TileID, TileName, TileDescription, PathToTileSheet, NumberOfTiles "
                            "FROM TileInMap, Tile "
                            "WHERE (Tile.TileID = TileInMap.TileID) AND (MapID = %1) "
                            "ORDER BY TileInMapIndex ASC").arg(MapID));

    if (!query.isActive())
    {
        qDebug() << "Query is not active";
    }

    return query;
}

QSqlQuery Database::getMapSprites(int MapID)
{
    QSqlQuery query(QString("SELECT Sprite.SpriteID, SpriteName, SpriteType, PathToSpriteSheet, PositionX, PositionY "
                            "FROM SpriteInMap, Sprite "
                            "WHERE (Sprite.SpriteID = SpriteInMap.SpriteID) AND (MapID = %1) "
                            "ORDER BY PositionX ASC, PositionY ASC").arg(MapID));
    return query;
}
