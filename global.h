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

const int BACKGROUND_LAYER  = 0;
const int TILE_LAYER        = 10;
const int SPRITE_LAYER      = 15;
const int PARTICLE_LAYER    = 40;
const int UI_LAYER          = 50;
const int RENDER_LAYER      = 60;

enum Action
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    SPRINT,
    WALK,
    SELECT,
    NONE
};

}

#endif // GLOBAL_H
