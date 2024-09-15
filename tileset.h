#ifndef TILESET_H
#define TILESET_H

#include <QList>
#include <QGraphicsRectItem>

struct HitboxInfo
{
    int m_x;
    int m_y;
    qreal m_width;
    qreal m_height;
    bool m_solid;
    bool m_interactable;
};

struct Hitbox : public QGraphicsRectItem
{
    Hitbox(QGraphicsItem* parent = 0) : QGraphicsRectItem(parent) {}
    bool m_solid;
    bool m_interactable;
};

struct TileInfo
{
    QString m_path;
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

    static const unsigned FLIPPED_HORIZONTALLY_FLAG  = 0x80000000;
    static const unsigned FLIPPED_VERTICALLY_FLAG    = 0x40000000;
    static const unsigned FLIPPED_DIAGONALLY_FLAG    = 0x20000000;

    QMap<int, TileInfo*> m_tiles;
    int m_firstGID;
};

#endif // TILESET_H
