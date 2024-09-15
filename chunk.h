#ifndef CHUNK_H
#define CHUNK_H

#include <QJsonArray>
#include <QTransform>
#include <QGraphicsPixmapItem>

#include "tileset.h"
#include "gameobject.h"

class Chunk : public QObject, public QGraphicsPixmapItem, public GameObject
{
    Q_OBJECT
public:
    // Constructors and destructors
    Chunk(QGraphicsItem * parent =0);
    ~Chunk();

    // Game loop functions
    virtual void clear(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void showHitboxes();
    void hideHitboxes();
    // Setters
    void setBgColour(QColor colour);
    void setData(QJsonArray data, QList<TileSet*> tilesets);

private:
    void setObjects(TileInfo* tile, QTransform transform, int x, int y);

    QColor m_bgColour;
    QList<Hitbox*> m_hitbox;
};

#endif // CHUNK_H
