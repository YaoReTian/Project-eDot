#ifndef TURNBASED_H
#define TURNBASED_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QQueue>

#include "Entities/player.h"
#include "Entities/combatsprite.h"
#include "UI/button.h"
#include "Utils/keymap.h"
#include "gameobject.h"

struct TurnBasedAction
{
    MoveType m_moveType;
    QList<CombatSprite*> m_targets;

};

class TurnBased : public GameObject
{
public:
    TurnBased();
    ~TurnBased();

    void setBackground(QPixmap backgroundImg);
    void setEvent(Player* player, QList<CombatSprite*> m_enemies);

    void input(KeyMap* keys);

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

private:
    QGraphicsPixmapItem* m_background;
    QQueue<CombatSprite*> m_spriteQueue;

    bool m_friendlyTurn;

    Button* m_basicButton;
    Button* m_skillButton;
};

#endif // TURNBASED_H
