#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QGraphicsScene>

class Tile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Tile(QGraphicsItem * parent=0);
    ~Tile();

    void setID(int TileID);
    void setName(QString TileName);
    void setDescription(QString TileDescription);
    void setSolid(bool value = true);
    bool isSolid();

    int getID();
    QString getName();
    QString getDescription();

    void removeItem(QGraphicsScene &scene);
    void render(QGraphicsScene &scene);

private:
    int m_TileID;
    QString m_TileName;
    QString m_TileDescription;

    bool m_solid;
};

#endif // TILE_H
