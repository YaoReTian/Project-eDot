#include "gamescene.h"

#include <QDebug>
#include <QString>

#include "database.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
{
    testOutput();
}

void GameScene::testOutput() {
    Database db;

    if (db.isOpen()) {
        QString filepath = db.getTestTilePath();
        qDebug() << filepath;
    } else {
        qDebug() << "Database is not open";
    }
}
