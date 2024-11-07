#ifndef TILESET_H
#define TILESET_H

#include <QList>
#include <QGraphicsRectItem>
#include <QPixmap>

enum TileType
{
    Wall,
    NOTYPE
};

struct Properties
{
    bool m_solid;
    bool m_interactable;
    TileType m_type = NOTYPE;
};

struct HitboxInfo : public Properties
{
    int m_x;
    int m_y;
    qreal m_width;
    qreal m_height;
    int m_mapLinkID = -1;
};

struct Hitbox : public QGraphicsRectItem, public Properties
{
    Hitbox(QGraphicsItem* parent = 0) : QGraphicsRectItem(parent) {}
    int m_mapLinkID;
};

struct TileInfo : public Properties
{
    QString m_path;
    QPixmap m_pixmap;
    qreal m_height;
    qreal m_width;
    QList<HitboxInfo*> m_hitboxes;
};

class TileSet
{
public:
    TileSet();
    TileSet(QString tileSetFilepath, int firstGID);

    void setData(QString tileSetFilepath);
    void setFirstGID(int firstGID);
    TileInfo* getInfo(int GID);
    int firstGID();

    static int findTilesetIndex(int GID, QList<TileSet*> tilesets);
    static std::tuple<qint64, QTransform> formatGID(qint64 unformatted_GID);

private:

    void parseOnImageCollection(QJsonArray tileArray);
    void parseOnTileSetImage(QJsonObject object);

    void checkProperties(TileInfo* tile, QJsonObject val);
    void setHitboxes(TileInfo* tile, QJsonObject val);


    static const unsigned FLIPPED_HORIZONTALLY_FLAG  = 0x80000000;
    static const unsigned FLIPPED_VERTICALLY_FLAG    = 0x40000000;
    static const unsigned FLIPPED_DIAGONALLY_FLAG    = 0x20000000;

    QMap<int, TileInfo*> m_tiles;
    int m_firstGID;
};

#endif // TILESET_H
