#include "tile.h"

Tile::Tile(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{
    qDebug() << "Tile created";
}

void Tile::setID(int TileID)
{
    m_TileID = TileID;
}

void Tile::setName(QString TileName)
{
    m_TileName = TileName;
}

void Tile::setDescription(QString TileDescription)
{
    m_TileDescription = TileDescription;
}

int Tile::getID()
{
    return m_TileID;
}

QString Tile::getName()
{
    return m_TileName;
}

QString Tile::getDescription()
{
    return m_TileDescription;
}
