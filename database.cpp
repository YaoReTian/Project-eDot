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
