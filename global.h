#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSize>
#include <QString>

namespace GLOBAL {

const int FPS               = 60;
const int TileLength        = 32;
const float Scale           = 2;
const int ObjectLength      = Scale * TileLength;
const QSize ObjectSize      = QSize(ObjectLength,ObjectLength);

const int BACKGROUND_LAYER  = 0;
const int TILE_LAYER        = 10;
const int SPRITE_LAYER      = 15;
const int ENTITY_LAYER      = 20;
const int PLAYER_LAYER      = 30;
const int PARTICLE_LAYER    = 40;
const int UI_LAYER          = 50;

enum Action
{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    SPRINT,
    NONE
};

}

#endif // GLOBAL_H
