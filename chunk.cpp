#include "chunk.h"
#include "Utils/global.h"

#include <QDebug>
#include <QPainter>

Chunk::Chunk(QGraphicsItem * parent)
    : QObject(), QGraphicsPixmapItem(parent), m_bgColour(Qt::transparent)
{}

Chunk::~Chunk()
{
    qDeleteAll(m_hitbox);
    m_hitbox.clear();
}

void Chunk::update(int deltatime)
{

}

void Chunk::setBgColour(QColor colour)
{
    m_bgColour = colour;
}

void Chunk::setObjects(TileInfo* tile, QTransform transform, int x, int y)
{
    if (tile->m_hitboxes.size() != 0)
    {
        for (const auto &o : std::as_const(tile->m_hitboxes))
        {
            m_hitbox.append(new Hitbox(this));
            m_hitbox.back()->setPen(QPen(Qt::transparent));
            m_hitbox.back()->m_solid = o->m_solid;
            m_hitbox.back()->m_type = o->m_type;
            m_hitbox.back()->m_mapLinkID = -1;
            m_hitbox.back()->m_interactable = o->m_interactable;
            m_hitbox.back()->setRect(o->m_x * GLOBAL::Scale - GLOBAL::ObjectLength/2,
                                     o->m_y * GLOBAL::Scale - GLOBAL::ObjectLength/2,
                                     o->m_width * GLOBAL::Scale,
                                     o->m_height * GLOBAL::Scale);
            m_hitbox.back()->setTransform(transform);
            m_hitbox.back()->setPos(x*GLOBAL::ObjectLength + GLOBAL::ObjectLength/2,
                                    y*GLOBAL::ObjectLength + GLOBAL::ObjectLength/2);
        }
    }
}

void Chunk::setData(QJsonArray data, QList<TileSet*> tilesets)
{
    int x = 0;
    int y = 0;
    int count16 = 1;

    // For generating images
    qint64 GID;
    QTransform transform;
    int ts;

    QPixmap pixmap(16*GLOBAL::ObjectLength, 16*GLOBAL::ObjectLength);
    pixmap.fill(m_bgColour);
    QPainter p;
    p.begin(&pixmap);

    for (const auto &val : std::as_const(data))
    {
        std::tie(GID, transform) = TileSet::formatGID(val.toInteger());

        if (GID != 0)
        {
            ts = TileSet::findTilesetIndex(GID, tilesets);
            TileInfo* tile = tilesets[ts]->getInfo(GID);
            p.drawPixmap(x*GLOBAL::ObjectLength,y*GLOBAL::ObjectLength,
                         tile->m_pixmap.scaled(GLOBAL::ObjectSize).transformed(transform));
            setObjects(tile, transform, x, y);
            tile = nullptr;
        }

        if (y > 15 || x > 15)
        {
            qDebug() << "ERROR: out of index";
            qDebug() << "y: " << y;
            qDebug() << "x: " << x;
        }

        if (count16 == 16)
        {
            y++;
            x = 0;
            count16 = 1;
        }
        else
        {
            x++;
            count16++;
        }
    }
    p.end();
    setPixmap(pixmap);
}

void Chunk::showHitboxes()
{
    for (const auto h : std::as_const(m_hitbox))
    {
        h->setPen(QPen(Qt::yellow));
    }
}

void Chunk::hideHitboxes()
{
    for (const auto h : std::as_const(m_hitbox))
    {
        h->setPen(QPen(Qt::transparent));
    }
}
