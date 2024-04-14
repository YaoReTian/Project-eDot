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
    NEXT_OPTION,
    PREV_OPTION,
    BASIC,
    SKILL,
    OVERDRIVE_1,
    OVERDRIVE_2,
    OVERDRIVE_3,
    OVERDRIVE_4,
    MOVE_TARGET_LEFT,
    MOVE_TARGET_RIGHT,
    MOVE_HAND_LEFT,
    MOVE_HAND_RIGHT,
    NONE
};

enum Element
{
    PHYSICAL,
    QUANTUM,
    IMAGINARY
};

/*
static QString ElementToString(Element element)
{
    switch (element)
    {
    case (PHYSICAL):
        return "Physical";
    case (QUANTUM):
        return "Quantum";
    case (IMAGINARY):
        return "Imaginary";
    }
    return "ERROR: NO TYPE FOUND";
};
*/

}

#endif // GLOBAL_H
