#ifndef TILELAYER_H
#define TILELAYER_H

#include "gameobject.h"
#include "chunk.h"
#include <QList>
#include <QJsonArray>
#include <QGraphicsScene>
#include <QGraphicsRectItem>

class TileLayer : public GameObject, public QGraphicsRectItem
{
public:
    TileLayer(QGraphicsItem * parent = 0);
    ~TileLayer();

    virtual void clear(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void setID(int id);
    void setOpacity(float opacity);
    void setName(QString name);
    void setBgColour(QColor colour);
    void setChunks(QJsonArray jsonChunks, QList<TileSet*> tilesets);

    int numberOfChunks();
    Chunk* getChunk(int index);
    int id();
    float opacity();
    QString name();

private:
    QList<Chunk*> m_chunks;
    QColor m_bgColour;

    int m_id;
    float m_opacity;
    QString m_name;
};

#endif // TILELAYER_H
