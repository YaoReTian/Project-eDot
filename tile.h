#ifndef TILE_H
#define TILE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsItem>

class Tile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Tile(QGraphicsItem * parent=0);

    void setID(int TileID);
    void setName(QString TileName);
    void setDescription(QString TileDescription);

    int getID();
    QString getName();
    QString getDescription();

private:
    int m_TileID;
    QString m_TileName;
    QString m_TileDescription;
};

#endif // TILE_H
