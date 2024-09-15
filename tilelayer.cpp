#include "tilelayer.h"

#include <QJsonObject>
#include "Utils/global.h"

TileLayer::TileLayer(QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_bgColour(Qt::transparent)
{}

TileLayer::~TileLayer()
{
    qDeleteAll(m_chunks);
}

void TileLayer::clear(QGraphicsScene &scene)
{
    for (const auto &chunk : std::as_const(m_chunks))
    {
        chunk->clear(scene);
    }
}

void TileLayer::update(int deltatime)
{

}

void TileLayer::render(QGraphicsScene &scene)
{
    for (const auto &chunk : std::as_const(m_chunks))
    {
        chunk->render(scene);
    }
}

void TileLayer::setBgColour(QColor colour)
{
    m_bgColour = colour;
}

void TileLayer::setChunks(QJsonArray jsonChunks, QList<TileSet*> tilesets)
{
    for (const auto &val : std::as_const(jsonChunks))
    {
        QJsonObject v = val.toObject();
        m_chunks.append(new Chunk(this));
        m_chunks.back()->setBgColour(m_bgColour);
        m_chunks.back()->setData(v.value("data").toArray(), tilesets);
        m_chunks.back()->setPos(v.value("x").toInt()*GLOBAL::ObjectLength,
                                v.value("y").toInt()*GLOBAL::ObjectLength);
    }
}

void TileLayer::setID(int id)
{
    m_id = id;
}

void TileLayer::setOpacity(float opacity)
{
    m_opacity = opacity;
}

void TileLayer::setName(QString name)
{
    m_name = name;
}

int TileLayer::numberOfChunks()
{
    return m_chunks.size();
}

Chunk* TileLayer::getChunk(int index)
{
    return m_chunks[index];
}

int TileLayer::id()
{
    return m_id;
}

float TileLayer::opacity()
{
    return m_opacity;
}

QString TileLayer::name()
{
    return m_name;
}
