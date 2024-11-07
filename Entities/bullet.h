#ifndef BULLET_H
#define BULLET_H

#include <QGraphicsPixmapItem>
#include "gameitem.h"
#include "../Utils/vectorfield.h"
#include "../tileset.h"

class Bullet : public GameItem
{
public:
    Bullet(TileSet* ts, QGraphicsItem* parent = 0);
    ~Bullet();
    void setTileset(TileSet* ts);
    void setFriendly(bool value = true);
    virtual void update(int deltaTime) override;

    void setUnitSpeed(qreal spd);
    void setPos(const QPointF &pos);
    void setPos(qreal x, qreal y);
    void addField(VectorField* field, QPointF origin);
    void addField(VectorField* field, qreal x, qreal y);
    void addField(VectorField *field, GameItem* originItem);
    void hide();
    void setDmg(int dmg);

    void setBulletID(int id);

    bool collided();
    bool isFriendly() const;
    qreal unitSpeed() const;
    int dmg() const;

private:
    int m_dmg;
    bool m_friendly;
    bool m_grazed;
    TileSet* m_ts;
    qreal m_unitSpeed; // scale factor for a unit vector
    QList<VectorField*> m_staticFields;
    QList<QPointF> m_origins;
    QList<VectorField*> m_dynamicFields;
    QList<GameItem*> m_originItems;
};

#endif // BULLET_H
