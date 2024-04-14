#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>

#include "gameobject.h"

class Tile : public QObject, public QGraphicsPixmapItem, public GameObject
{
    Q_OBJECT
public:
    Tile(QGraphicsItem * parent=0);
    ~Tile();

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void setID(int TileID);
    void setName(QString TileName);
    void setDescription(QString TileDescription);
    void setSolid(bool value = true);
    bool isSolid();

    int getID();
    QString getName();
    QString getDescription();

private:
    int m_TileID;
    QString m_TileName;
    QString m_TileDescription;

    bool m_solid;
};

#endif // TILE_H
