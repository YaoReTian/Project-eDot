#include "gamescene.h"

#include <QDebug>
#include <QString>

#include "database.h"

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void GameScene::testOutput() {
    Database db;
    QString filepath;

    if (db.isOpen()) {
        filepath = db.getTestTilePath();
        qDebug() << filepath;
    } else {
        qDebug() << "Database is not open";
    }
}
