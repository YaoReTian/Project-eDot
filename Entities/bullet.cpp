#include "bullet.h"
#include "../Utils/global.h"

#include "../tileset.h"

Bullet::Bullet(QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), Vector(), m_dmg(1), m_friendly(false)
{
    TileSet t(":/tileset/res/Basic bullets.tsj",1);
    setPixmap(t.getInfo(182)->m_pixmap);

}

Bullet::~Bullet()
{
}

void Bullet::update(int deltaTime)
{
    setPos(x() + i() * GLOBAL::ObjectLength * deltaTime,
           y() + j() * GLOBAL::ObjectLength * deltaTime);
    setTransformOriginPoint(boundingRect().width()/2, boundingRect().height()/2);
    setRotation(angle());
}
