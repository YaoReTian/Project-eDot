#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include "database.h"
#include "UI/userinterface.h"
#include "gameobject.h"

#include "tilelayer.h"
#include "tileset.h"
#include "Utils/keymap.h"

class Tilemap : public GameObject, public QGraphicsRectItem
{
public:
    Tilemap(QGraphicsItem* parent = 0);
    ~Tilemap();
    void setDatabase(Database* db);
    void setMap(int MapID, UserInterface* UI);
    QString getMapName();
    QString getMapDesc();
    int getMapSizeX();
    int getMapSizeY();
    qreal getPlayerZ();
    QRgb bgColour();

    void input(KeyMap* keys);
    virtual void clear(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void generateTiles(QGraphicsScene &scene);
    void generateSprites(QGraphicsScene &scene);

private:
    // Methods for creating the map
    void createTileLayer(QJsonObject chunks, int zValue);
    void createSpriteLayer(QJsonObject objectGroup, int zValue);
    void setSprites(UserInterface* UI);

    // Attributes for creating the map
    Database* m_db;

    // Attributes of objects on the map
    QList<Sprite*> m_sprites;
    //Tile* m_tiles[100][100];

    // Map info
    int m_mapID;
    QString m_mapName;
    QString m_mapDesc;
    int m_mapSizeX;
    int m_mapSizeY;
    qreal m_playerZ;

    // Flags
    int m_enteredCombatIndex;

    // TESTING
    QRgb m_backgroundColour;
    bool m_infinite;
    QList<TileLayer*> m_layers;
    QList<TileSet*> m_tilesets;
    bool m_hitboxesVisible;
};

#endif // MAP_H
