#include "tileset.h"

#include "Utils/jsonparser.h"
#include <QJsonArray>

TileSet::TileSet() {}

TileSet::TileSet(QString tileSetFilepath, int firstGID)
    : m_firstGID(firstGID)
{
    setData(tileSetFilepath);
}

TileSet::~TileSet()
{
    qDeleteAll(m_tiles);
    m_tiles.clear();
}

void TileSet::setData(QString tileSetFilepath)
{
    qDeleteAll(m_tiles);
    m_tiles.clear();
    QJsonObject tileset = JsonParser::parse(tileSetFilepath);
    if (tileset.value("image").isUndefined())
    {
        parseOnImageCollection(tileset.value("tiles").toArray());
    }
    else
    {
        parseOnTileSetImage(tileset);
    }
}

void TileSet::parseOnImageCollection(QJsonArray tileArray)
{
    int i;
    for (const auto &t : std::as_const(tileArray))
    {
        QJsonObject val = t.toObject();
        i = val.value("id").toInt();
        m_tiles[i] = new TileInfo;
        m_tiles[i]->m_height = val.value("imageheight").toInt();
        m_tiles[i]->m_width = val.value("imagewidth").toInt();
        m_tiles[i]->m_path = JsonParser::parsePath(val.value("image").toString());
        m_tiles[i]->m_pixmap = QPixmap(m_tiles[i]->m_path);

        if (!val.value("type").isUndefined())
        {
            QString type = val.value("type").toString();
            if (type == "Wall")
            {
                m_tiles[i]->m_type = Wall;
            }
        }

        if (!val.value("properties").isUndefined())
        {
            checkProperties(m_tiles[i], val);
        }

        if (!val.value("objectgroup").isUndefined())
        {
            setHitboxes(m_tiles[i], val);
        }
    }
}

void TileSet::parseOnTileSetImage(QJsonObject object)
{
    QPixmap pix(JsonParser::parsePath(object.value("image").toString()));
    int tileH = object.value("tileheight").toInt();
    int tileW = object.value("tilewidth").toInt();
    int tilesX = object.value("imagewidth").toInt() / tileW;
    int tilesY = object.value("imageheight").toInt() / tileH;
    int i = 0;
    for (int y = 0; y < tilesY; y++)
    {
        for (int x = 0; x < tilesX; x++)
        {
            m_tiles[i] = new TileInfo;
            m_tiles[i]->m_height = tileH;
            m_tiles[i]->m_width = tileW;
            m_tiles[i]->m_path = "";
            m_tiles[i]->m_pixmap = pix.copy(x*tileW, y*tileH, tileW, tileH);
            i++;
        }
    }
    if (!object.value("tiles").isUndefined())
    {
        QJsonArray data = object.value("tiles").toArray();
        for (const auto t : data)
        {
            QJsonObject tile = t.toObject();
            int id = tile.value("id").toInt();
            if (!tile.value("properties").isUndefined())
            {
                checkProperties(m_tiles[id], tile);
            }

            if (!tile.value("objectgroup").isUndefined())
            {
                setHitboxes(m_tiles[id], tile);
            }
        }
    }
}

void TileSet::checkProperties(TileInfo* tile, QJsonObject val)
{
    QJsonArray properties = val.value("properties").toArray();
    tile->m_interactable = properties[0].toObject().value("value").toBool();
    tile->m_solid = properties[1].toObject().value("value").toBool();

}

void TileSet::setHitboxes(TileInfo *tile, QJsonObject val)
{
    QJsonArray objects = val.value("objectgroup").toObject().value("objects").toArray();
    for (const auto &o : std::as_const(objects))
    {
        QJsonObject obj = o.toObject();
        tile->m_hitboxes.append(new HitboxInfo);
        tile->m_hitboxes.back()->m_x= obj.value("x").toDouble();
        tile->m_hitboxes.back()->m_y = obj.value("y").toDouble();
        tile->m_hitboxes.back()->m_width = obj.value("width").toDouble();
        tile->m_hitboxes.back()->m_height = obj.value("height").toDouble();
        tile->m_hitboxes.back()->m_interactable = tile->m_interactable;
        tile->m_hitboxes.back()->m_solid = tile->m_solid;
        tile->m_hitboxes.back()->m_type = tile->m_type;
    }
}

TileInfo* TileSet::getInfo(int GID)
{
    if (GID < m_firstGID)
    {
        qDebug() << "Error: GID < firstGID";
        return nullptr;
    }
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
    if (GID == 0)
    {
        qDebug() << "ERROR: Invalid GID";
        return -1;
    }

    int index = -1;

    for (const auto &set : tilesets)
    {
        if (set->firstGID() > GID)
        {
            return index;
        }
        index++;
    }
    return index;
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
