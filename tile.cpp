#include "tile.h"

#include <QRandomGenerator>

Tile::Tile(QGraphicsItem *parent)
    : QObject(), QGraphicsPixmapItem(parent), m_TileID(-1), m_TileName("Unset"),
    m_TileDescription("Unset"), m_solid(false)
{
}

Tile::~Tile()
{

}

void Tile::removeItem(QGraphicsScene &scene)
{
    scene.removeItem(this);
}

void Tile::update(int deltatime)
{

}


void Tile::render(QGraphicsScene &scene)
{
    scene.addItem(this);
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
