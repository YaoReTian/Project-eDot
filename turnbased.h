#ifndef TURNBASED_H
#define TURNBASED_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QQueue>

#include "Entities/player.h"
#include "Entities/combatsprite.h"

class TurnBased
{
public:
    TurnBased();

    void setBackground(QPixmap backgroundImg);
    void setEvent(Player* player, QList<CombatSprite*> m_enemies);

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime);
    void render(QGraphicsScene &scene);

private:
    QGraphicsPixmapItem* m_background;
    QQueue<CombatSprite*> m_spriteQueue;
};

#endif // TURNBASED_H
