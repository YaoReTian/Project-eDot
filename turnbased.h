#ifndef TURNBASED_H
#define TURNBASED_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QQueue>

#include "Entities/player.h"
#include "Entities/combatsprite.h"
#include "UI/button.h"
#include "Utils/keymap.h"

class TurnBased
{
public:
    TurnBased();

    void setBackground(QPixmap backgroundImg);
    void setEvent(Player* player, QList<CombatSprite*> m_enemies);

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime, KeyMap* keys);
    void render(QGraphicsScene &scene);

private:
    QGraphicsPixmapItem* m_background;
    QQueue<CombatSprite*> m_spriteQueue;

    bool m_friendlyTurn;

    Button* m_basicButton;
    Button* m_skillButton;
};

#endif // TURNBASED_H
