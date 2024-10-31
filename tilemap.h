#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "database.h"
#include "gameobject.h"

#include "tilelayer.h"
#include "tileset.h"
#include "Utils/keymap.h"
#include "Entities/bulletmanager.h"
#include "Entities/enemy.h"

class Tilemap : public GameObject, public QGraphicsRectItem
{
public:
    Tilemap(QGraphicsItem* parent = 0);
    ~Tilemap();
    void setDatabase(Database* db);
    void setBulletManager(BulletManager* manager);
    void setMap(int MapID);
    QString getMapName();
    QString getMapDesc();
    int getMapSizeX();
    int getMapSizeY();
    qreal getPlayerZ();
    QRgb bgColour();

    void input(KeyMap* keys);
    virtual void update(int deltatime) override;

private:
    // Methods for creating the map
    void createTileLayer(QJsonObject chunks, int zValue);
    void createSpriteLayer(QJsonObject objectGroup, int zValue);

    // Utilities
    Database* m_db;
    BulletManager* m_bulletManager;

    // Attributes of objects on the map
    QList<Sprite*> m_sprites;
    QList<Enemy*> m_enemies;

    // Map info
    int m_mapID;
    QString m_mapName;
    QString m_mapDesc;
    int m_mapSizeX;
    int m_mapSizeY;
    qreal m_playerZ;
    QRgb m_backgroundColour;
    bool m_infinite;
    QList<TileLayer*> m_layers;
    QList<TileSet*> m_tilesets;

    // Flags
    bool m_hitboxesVisible;
};

#endif // MAP_H
