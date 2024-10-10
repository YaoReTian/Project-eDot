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
    SPRINT,
    WALK,
    SELECT,
    NEXT_OPTION,
    PREV_OPTION,
    OVERDRIVE_1,
    OVERDRIVE_2,
    OVERDRIVE_3,
    OVERDRIVE_4,
    SHOW_HITBOX,
    SHOOT,
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
