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
    DEAD,
    ANIMATION_DONE,
    ANY_OTHER,
    HEAL,
    ENTER,
    DELETE,
    NONE
};
}

#endif // GLOBAL_H
