#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSize>
#include <QString>
#include <QFont>

namespace GLOBAL {

const int FPS               = 60;
const int TileLength        = 32;
const float Scale           = 2;
const int ObjectLength      = Scale * TileLength;
const QSize ObjectSize      = QSize(ObjectLength,ObjectLength);

enum Action
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    SELECT,
    NEXT_OPTION,
    OVERDRIVE,
    SHOW_HITBOX,
    SHOW_PLAYER_HITBOX,
    SHOOT,
    PAUSE,
    NONE
};

/*
static QString ActionToString(Action a)
{
    switch (a)
    {
    case MOVE_LEFT:
        return "Move left";
    case MOVE_RIGHT:
        return "Move right";
    case MOVE_UP:
        return "Move up";
    case MOVE_DOWN:
        return "Move down";
    case SELECT:
        return "Select";
    case NEXT_OPTION:
        return "Next option";
    case OVERDRIVE:
        return "Quantum Overdrive";
    case SHOW_HITBOX:
        return "Show wall collision hitboxes";
    case SHOW_PLAYER_HITBOX:
        return "Slow / Show player hitbox";
    case SHOOT:
        return "Shoot";
    case PAUSE:
        return "Pause";
    }

    return "ERROR: NO TYPE FOUND";
};
*/

}

#endif // GLOBAL_H
