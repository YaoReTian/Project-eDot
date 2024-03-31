#include "tile.h"
#include "Entities/movingsprite.h"

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

void Tile::update()
{
    if (m_solid)
    {
        QList<QGraphicsItem*> list = collidingItems();
        for (auto n : list)
        {
            if (typeid(*n) == typeid(MovingSprite)) blockSprite(n);
        }
    }
}

void Tile::blockSprite(QGraphicsItem* item)
{
    bool north = false;
    bool south = false;
    bool east = false;
    bool west = false;
    qreal value = QRandomGenerator::global()->generateDouble() * 0.2f;

    float item_mid_x = item->x() + item->boundingRect().width()/2;
    float item_mid_y = item->y() + item->boundingRect().height()/2;

    if (item->y() + item->boundingRect().height() >= y() &&
        item_mid_y < y())
    {
        north = true;
    }

    if (item->y() <= y() + boundingRect().height() &&
        item_mid_y + item->boundingRect().height() + 2*GLOBAL::Scale > y() + boundingRect().height())
    {
        south = true;
    }

    if (item->x() <= x() + boundingRect().width() &&
        item_mid_x > x() + boundingRect().width())
    {
        east = true;
    }

    if (item->x() + item->boundingRect().width() >= x() &&
        item_mid_x < x())
    {
        west = true;
    }

    if (north && east)
    {
        if (abs(item_mid_y - y()) > abs(item_mid_x - (x() + boundingRect().width())))
        {
            east = false;
        }
        else
        {
            north = false;
        }
    }
    else if (north && west)
    {
        if (abs(item_mid_y - y()) > abs(item_mid_x - x()))
        {
            west = false;
        }
        else
        {
            north = false;
        }
    }
    else if (south && east)
    {
        if (abs(item_mid_y + item->boundingRect().height() + 2*GLOBAL::Scale - (y() + boundingRect().height())) >
            abs(item_mid_x - (x() + boundingRect().width())))
        {
            east = false;
        }
        else
        {
            south = false;
        }
    }
    else if (south && west)
    {
        if (abs(item_mid_y + item->boundingRect().height() + 2*GLOBAL::Scale - (y() + boundingRect().height())) >
            abs(item_mid_x - (x())))
        {
            west = false;
        }
        else
        {
            south = false;
        }
    }

    if (north)
    {
        item->setPos(item->x(), y() - item->boundingRect().height() - value);
    }
    else if (south)
    {
        item->setPos(item->x(), y() + boundingRect().height() + 2*GLOBAL::Scale - item->boundingRect().height() + value);
    }
    else if (east)
    {
        item->setPos(x() + boundingRect().width() + value, item->y());
    }
    else if (west)
    {
        item->setPos(x() - item->boundingRect().width() - value, item->y());
    }
}
