#include "database.h"

#include <QDebug>

#include "Entities/sprite.h"
#include <QSqlError>

#include <QFile>


Database::Database()
{
    if (!QFile::exists("GameDatabase.db"))
    {
        if (!QFile::copy(":/database/GameDatabase.db", "GameDatabase.db"))
        {
            qDebug() << "ERROR: unable to generate initial database";
        }
        QFile("GameDatabase.db").setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
    }
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("GameDatabase.db");

    if (!m_db.open()) {
        qDebug() << "Error: database failed to open";
    }
    else {
        qDebug() << "Database opened";
    }

    for (int i = 0; i < 5; i++)
    {
        m_saveSlots[i] = new SaveSlot;
    }

    QSqlQuery query("SELECT * FROM PlayerData");

    int i;
    while (query.next())
    {
        i = query.value("PlayerID").toInt() - 1;
        m_saveSlots[i]->m_username = query.value("Username").toString();
        m_saveSlots[i]->m_tilePosX = query.value("PosX").toInt();
        m_saveSlots[i]->m_tilePosY = query.value("PosY").toInt();
        m_saveSlots[i]->m_mapID = query.value("MapID").toInt();
        m_saveSlots[i]->m_HP = query.value("HP").toInt();
        m_saveSlots[i]->m_charge = query.value("Charge").toInt();
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
                                  animationQuery.value("Line").toInt(),
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
                                  animationQuery.value("Line").toInt(),
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

BulletManager* Database::getBulletManager()
{
    QSqlQuery query(QString("SELECT * FROM Field"));
    BulletManager* b  = new BulletManager;
    while (query.next())
    {
        VectorField* v = new VectorField;
        v->setField(query.value("i").toString(), query.value("j").toString());
        b->addField(v, query.value("FieldKey").toString());
    }
    return b;
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
                                  animationQuery.value("Line").toInt(),
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

void Database::setEnemyData(QString path, Enemy *source)
{
    setSpriteData(path, source);
    QSqlQuery query(QString("SELECT * FROM Phase WHERE (SpriteID = %1)").arg(source->getID()));

    while(query.next())
    {
        Phase* p = new Phase;
        p->m_hp = query.value("HP").toInt();
        p->m_phaseTime = query.value("PhaseTime").toInt();
        QSqlQuery patterns(QString("SELECT * FROM PhasePattern, Pattern "
                                   "WHERE (PhasePattern.PatternID = Pattern.PatternID) "
                                   "AND (PhaseID = %1)").arg(query.value("PhaseID").toInt()));

        while (patterns.next())
        {
            p->m_patterns.append(new Pattern);
            p->m_patterns.back()->m_spawnRate = patterns.value("BulletSpawnRate").toInt();
            p->m_patterns.back()->m_fieldKey = patterns.value("FieldKey").toString();
            p->m_patterns.back()->m_fieldOrigin.setX(patterns.value("PhasePattern.PosX").toInt());
            p->m_patterns.back()->m_fieldOrigin.setY(patterns.value("PhasePattern.PosY").toInt());
            p->m_patterns.back()->m_unitSpeed = patterns.value("UnitSpeed").toDouble() / 1000.0f;
            QSqlQuery spawnPos(QString("SELECT * FROM BulletPatternPos, BulletInPattern "
                                      "WHERE (BulletPatternPos.BulletInPatternID = BulletInPattern.BulletInPatternID) "
                                      "AND (PatternID = %1)").arg(patterns.value("PatternID").toInt()));
            while (spawnPos.next())
            {
                p->m_patterns.back()->m_bulletIDs.append(spawnPos.value("BulletID").toInt());
                p->m_patterns.back()->m_spawnPos.append(QPointF(spawnPos.value("PosX").toInt(), spawnPos.value("PosY").toInt()));
            }
        }
        source->addPhase(p);
    }
}

QSqlQuery Database::getSpriteAnimations(int SpriteID)
{
    QSqlQuery query(QString("SELECT StateName, Line, FrameTime "
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
    else if (string == "SHOW_PLAYER_HITBOX")
    {
        return GLOBAL::SHOW_PLAYER_HITBOX;
    }
    else if (string == "OVERDRIVE")
    {
        return GLOBAL::OVERDRIVE;
    }
    else if (string == "SHOOT")
    {
        return GLOBAL::SHOOT;
    }
    else if (string == "DEAD")
    {
        return GLOBAL::DEAD;
    }
    else if (string == "ANIMATION_DONE")
    {
        return GLOBAL::ANIMATION_DONE;
    }
    else if (string == "ANY_OTHER")
    {
        return GLOBAL::ANY_OTHER;
    }
    else if (string == "HEAL")
    {
        return GLOBAL::HEAL;
    }
    else
    {
        return GLOBAL::NONE;
    }
}

void Database::saveGame(int slotID, QString username, qreal tilePosX, qreal tilePosY, int mapID, int HP, int charge)
{
    if (slotID > 5 || slotID < 1)
    {
        qDebug() << "ERROR: UNREGISTERED SAVE SLOT";
        return;
    }
    m_saveSlots[slotID-1]->m_username = username;
    m_saveSlots[slotID-1]->m_tilePosX = tilePosX;
    m_saveSlots[slotID-1]->m_tilePosY = tilePosY;
    m_saveSlots[slotID-1]->m_mapID = mapID;
    if(m_db.transaction())
    {
        QString q("UPDATE PlayerData "
                  "SET Username = '%1', PosX = %2, PosY = %3, MapID = %4, HP = %5, Charge = %6 "
                  "WHERE PlayerID = %7");
        QSqlQuery query(q.arg(username).arg(tilePosX).arg(tilePosY).arg(mapID).arg(HP).arg(charge).arg(slotID));
        if (!query.isActive())
        {
            qDebug() << "ERROR: query inactive";
            qDebug() << query.lastError().text();
        }

        if(!m_db.commit())
        {
            qDebug() << "ERROR: Failed to commit";
            m_db.rollback();
        }
    }
    else
    {
        qDebug() << "ERROR: Failed to start transaction mode";
    }
}

SaveSlot* Database::getSaveSlot(int slotID)
{
    if (slotID > 5 || slotID < 1)
    {
        qDebug() << "ERROR: UNREGISTERED SAVE SLOT";
        return nullptr;
    }
    return m_saveSlots[slotID-1];
}
