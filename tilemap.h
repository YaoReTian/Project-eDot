#ifndef MAP_H
#define MAP_H

#include <QGraphicsScene>

#include "tile.h"
#include "database.h"
#include "UI/userinterface.h"
#include "Entities/combatsprite.h"
#include "gameobject.h"

class Tilemap : public GameObject
{
public:
    Tilemap();
    ~Tilemap();
    void setDatabase(Database* db);
    void setMap(int MapID, UserInterface* UI);
    QString getMapName();
    QString getMapDesc();
    int getMapSizeX();
    int getMapSizeY();
    bool enteredCombat();
    QList<CombatSprite*> getCombatSprites();

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void generateTiles(QGraphicsScene &scene);
    void generateSprites(QGraphicsScene &scene);

private:
    // Methods for creating the map
    void setTiles();
    void setSprites(UserInterface* UI);

    // Attributes for creating the map
    Database* m_db;

    // Attributes of objects on the map
    QList<InteractiveSprite*> m_sprites;
    QList<Tile*> m_tiles;

    // Map info
    int m_mapID;
    QString m_mapName;
    QString m_mapDesc;
    int m_mapSizeX;
    int m_mapSizeY;

    // Flags
    int m_enteredCombatIndex;
};

#endif // MAP_H
