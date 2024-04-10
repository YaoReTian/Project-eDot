#include "tile.h"

#include <QRandomGenerator>

Tile::Tile(QGraphicsItem *parent) : QObject(), QGraphicsPixmapItem(parent)
{

}

Tile::~Tile()
{
    qDebug() << "Item destroyed";
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

void Tile::setSolid(bool value)
{
    m_solid = value;
}

bool Tile::isSolid()
{
    return m_solid;
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

void Tile::removeItem(QGraphicsScene &scene)
{
    scene.removeItem(this);
}

void Tile::render(QGraphicsScene &scene)
{
    scene.addItem(this);
}
