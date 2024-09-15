#include "tileset.h"

#include "Utils/jsonparser.h"
#include <QJsonArray>

TileSet::TileSet() {}

TileSet::TileSet(QString tileSetFilepath, int firstGID)
    : m_firstGID(firstGID)
{
    setData(tileSetFilepath);
}

void TileSet::setData(QString tileSetFilepath)
{
    bool interactable;
    bool solid;
    int i;

    QJsonArray tiles = JsonParser::parse(tileSetFilepath).value("tiles").toArray();
    for (const auto &t : std::as_const(tiles))
    {
        interactable = false;
        solid = false;

        QJsonObject val = t.toObject();
        i = val.value("id").toInt();
        m_tiles[i] = new TileInfo;
        m_tiles[i]->m_height = val.value("imageheight").toInt();
        m_tiles[i]->m_width = val.value("imagewidth").toInt();
        m_tiles[i]->m_path = JsonParser::parsePath(val.value("image").toString());

        if (!val.value("properties").isUndefined())
        {
            QJsonArray properties = val.value("properties").toArray();
            interactable = properties[0].toObject().value("value").toBool();
            solid = properties[1].toObject().value("value").toBool();
        }

        if (!val.value("objectgroup").isUndefined())
        {
            QJsonArray objects = val.value("objectgroup").toObject().value("objects").toArray();
            for (const auto &o : std::as_const(objects))
            {
                QJsonObject obj = o.toObject();
                m_tiles[i]->m_hitboxes.append(new HitboxInfo);
                m_tiles[i]->m_hitboxes.back()->m_x= obj.value("x").toDouble();
                m_tiles[i]->m_hitboxes.back()->m_y = obj.value("y").toDouble();
                m_tiles[i]->m_hitboxes.back()->m_width = obj.value("width").toDouble();
                m_tiles[i]->m_hitboxes.back()->m_height = obj.value("height").toDouble();
                m_tiles[i]->m_hitboxes.back()->m_interactable = interactable;
                m_tiles[i]->m_hitboxes.back()->m_solid = solid;
            }
        }
    }
}

TileInfo* TileSet::getInfo(int GID)
{

    return m_tiles[GID-m_firstGID];
}

void TileSet::setFirstGID(int firstGID)
{
    m_firstGID = firstGID;
}

int TileSet::firstGID()
{
    return m_firstGID;
}

int TileSet::findTilesetIndex(int GID, QList<TileSet*> tilesets)
{
    int index = 0;

    for (const auto &set : tilesets)
    {
        if (GID < set->firstGID())  return index;
        index++;
    }
    return index - 1;
}

std::tuple<qint64, QTransform> TileSet::formatGID(qint64 unformatted_GID)
{
    qint64 GID = 0;
    unsigned flag = 0;

    QTransform transform(1,0,0,1,0,0);
    if ((unformatted_GID & FLIPPED_VERTICALLY_FLAG) == FLIPPED_VERTICALLY_FLAG)
    {
        flag = flag ^ FLIPPED_VERTICALLY_FLAG;
        transform *= QTransform(1,0,0,-1,0,0);
    }
    if ((unformatted_GID & FLIPPED_HORIZONTALLY_FLAG) == FLIPPED_HORIZONTALLY_FLAG)
    {
        flag = flag ^ FLIPPED_HORIZONTALLY_FLAG;
        transform *= QTransform(-1,0,0,1,0,0);
    }
    if ((unformatted_GID & FLIPPED_DIAGONALLY_FLAG) == FLIPPED_DIAGONALLY_FLAG)
    {
        flag = flag  ^ FLIPPED_DIAGONALLY_FLAG;
        transform *= QTransform(0,-1,-1,0,0,0);
    }

    GID = unformatted_GID ^ flag;
    return {GID, transform};
}
