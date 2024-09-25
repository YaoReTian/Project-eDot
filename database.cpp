#include "database.h"

#include <QDebug>

#include "Entities/sprite.h"

#include <QFile>


Database::Database()
{
    if (!QFile::exists("GameDatabase.db"))
    {
        if (!QFile::copy(":/database/GameDatabase.db", "GameDatabase.db"))
        {
            qDebug() << "ERROR: unable to generate initial database";
        }
    }
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

Sprite* Database::getSprite(int SpriteID)
{
    QSqlQuery query(QString("SELECT * FROM Sprite "
                            "WHERE (SpriteID = %1)").arg(SpriteID));

    QSqlQuery animationQuery;
    QSqlQuery transitionQuery;
    QPixmap image;
    Sprite* sprite = new Sprite;

    query.next();
    sprite->setID(query.value("Sprite.SpriteID").toInt());
    sprite->setName(query.value("SpriteName").toString());
    sprite->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                                       query.value("SpriteSizeY").toInt()));
    image = QPixmap(query.value("PathToSpriteSheet").toString());
    sprite->setSpriteSheet(image);

    animationQuery = getSpriteAnimations(sprite->getID());
    while (animationQuery.next()) {
        sprite->addAnimationState(animationQuery.value("StateName").toString(),
                                  animationQuery.value("StartFrame").toInt(),
                                  animationQuery.value("EndFrame").toInt(),
                                  animationQuery.value("FrameTime").toFloat());

        transitionQuery = getSpriteTransitions(sprite->getID(),
                                               animationQuery.value("StateName").toString());
        while (transitionQuery.next())
        {
            sprite->addTransition(animationQuery.value("StateName").toString(),
                                  stringToAction(transitionQuery.value("Action").toString()),
                                  transitionQuery.value("EndStateName").toString());
        }
    }

    return sprite;
}

Sprite* Database::getSprite(QString path, QGraphicsItem* parent)
{
    QSqlQuery query(QString("SELECT * FROM Sprite "
                            "WHERE (PathToSpriteSheet = '%1')").arg(path));

    QSqlQuery animationQuery;
    QSqlQuery transitionQuery;
    QPixmap image;
    Sprite* sprite = new Sprite(parent);

    query.next();
    sprite->setID(query.value("Sprite.SpriteID").toInt());
    sprite->setName(query.value("SpriteName").toString());
    sprite->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                               query.value("SpriteSizeY").toInt()));
    image = QPixmap(query.value("PathToSpriteSheet").toString());
    sprite->setSpriteSheet(image);

    animationQuery = getSpriteAnimations(sprite->getID());
    while (animationQuery.next()) {
        sprite->addAnimationState(animationQuery.value("StateName").toString(),
                                  animationQuery.value("StartFrame").toInt(),
                                  animationQuery.value("EndFrame").toInt(),
                                  animationQuery.value("FrameTime").toFloat());

        transitionQuery = getSpriteTransitions(sprite->getID(),
                                               animationQuery.value("StateName").toString());
        while (transitionQuery.next())
        {
            sprite->addTransition(animationQuery.value("StateName").toString(),
                                  stringToAction(transitionQuery.value("Action").toString()),
                                  transitionQuery.value("EndStateName").toString());
        }
    }
    return sprite;
}

void Database::setSpriteData(QString path, Sprite* source)
{
    QSqlQuery query(QString("SELECT * FROM Sprite "
                            "WHERE (PathToSpriteSheet = '%1')").arg(path));

    QSqlQuery animationQuery;
    QSqlQuery transitionQuery;
    QPixmap image;

    query.next();
    source->setID(query.value("Sprite.SpriteID").toInt());
    source->setName(query.value("SpriteName").toString());
    source->setFrameSize(QSize(query.value("SpriteSizeX").toInt(),
                               query.value("SpriteSizeY").toInt()));
    image = QPixmap(query.value("PathToSpriteSheet").toString());
    source->setSpriteSheet(image);

    animationQuery = getSpriteAnimations(source->getID());
    while (animationQuery.next()) {
        source->addAnimationState(animationQuery.value("StateName").toString(),
                                  animationQuery.value("StartFrame").toInt(),
                                  animationQuery.value("EndFrame").toInt(),
                                  animationQuery.value("FrameTime").toFloat());

        transitionQuery = getSpriteTransitions(source->getID(),
                                               animationQuery.value("StateName").toString());
        while (transitionQuery.next())
        {
            source->addTransition(animationQuery.value("StateName").toString(),
                                  stringToAction(transitionQuery.value("Action").toString()),
                                  transitionQuery.value("EndStateName").toString());
        }
    }
}

QSqlQuery Database::getSpriteAnimations(int SpriteID)
{
    QSqlQuery query(QString("SELECT StateName, StartFrame, EndFrame, FrameTime "
                            "FROM SpriteAnimations "
                            "WHERE (SpriteID = %1)").arg(SpriteID));
    return query;
}
QSqlQuery Database::getSpriteTransitions(int SpriteID, QString startStateName)
{
    QSqlQuery query(QString("SELECT Action, EndStateName "
                            "FROM StateTransition, SpriteAnimations "
                            "WHERE (StartStateName = StateName) "
                            "AND (StateTransition.SpriteID = %1) "
                            "AND (StartStateName = '%2')").arg(SpriteID).arg(startStateName));
    return query;
}

GLOBAL::Action Database::stringToAction(QString string)
{
    if (string == "MOVE_LEFT")
    {
        return GLOBAL::MOVE_LEFT;
    }
    else if (string == "MOVE_RIGHT")
    {
        return GLOBAL::MOVE_RIGHT;
    }
    else if (string == "MOVE_UP")
    {
        return GLOBAL::MOVE_UP;
    }
    else if (string == "MOVE_DOWN")
    {
        return GLOBAL::MOVE_DOWN;
    }
    else
    {
        return GLOBAL::NONE;
    }
}
