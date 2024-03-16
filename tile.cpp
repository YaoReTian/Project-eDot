#include "tile.h"
#include "movingsprite.h"

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

    QList<QGraphicsItem*> list = collidingItems();

    for (auto n : list)
    {
        if (typeid(*n) == typeid(MovingSprite))
        {
            blockSprite(n);
        }
    }
}

void Tile::blockSprite(QGraphicsItem* item)
{
    bool north_out = false;
    bool south_out = false;
    bool east_out = false;
    bool west_out = false;
    qreal value = QRandomGenerator::global()->generateDouble() * 0.2f;

    float item_mid_x = item->x() + item->boundingRect().width()/2;
    float item_mid_y = item->y() + item->boundingRect().height()/2;

    if (item->y() + item->boundingRect().height() >= y() &&
        item_mid_y < y())
    {
        north_out = true;
    }

    if (item->y() <= y() + boundingRect().height() &&
        item_mid_y > y() + boundingRect().height())
    {
        south_out = true;
    }

    if (item->x() <= x() + boundingRect().width() &&
        item_mid_x > x() + boundingRect().width())
    {
        east_out = true;
    }

    if (item->x() + item->boundingRect().width() >= x() &&
        item_mid_x < x())
    {
        west_out = true;
    }

    if (north_out && east_out)
    {
        if (abs(item_mid_y - y()) > abs(item_mid_x - (x() + boundingRect().width())))
        {
            east_out = false;
        }
        else
        {
            north_out = false;
        }
    }
    else if (north_out && west_out)
    {
        if (abs(item_mid_y - y()) > abs(item_mid_x - x()))
        {
            west_out = false;
        }
        else
        {
            north_out = false;
        }
    }
    else if (south_out && east_out)
    {
        if (abs(item_mid_y - (y() + boundingRect().height())) > abs(item_mid_x - (x() + boundingRect().width())))
        {
            east_out = false;
        }
        else
        {
            south_out = false;
        }
    }
    else if (south_out && west_out)
    {
        if (abs(item_mid_y - (y() + boundingRect().height())) > abs(item_mid_x - (x())))
        {
            west_out = false;
        }
        else
        {
            south_out = false;
        }
    }

    if (north_out)
    {
        item->setPos(item->x(), y() - item->boundingRect().height() - value);
    }
    else if (south_out)
    {
        item->setPos(item->x(), y() + boundingRect().height() + value);
    }
    else if (east_out)
    {
        item->setPos(x() + boundingRect().width() + value, item->y());
    }
    else if (west_out)
    {
        item->setPos(x() - item->boundingRect().width() - value, item->y());
    }
}
