#ifndef GLOBAL_H
#define GLOBAL_H

#include <QSize>
#include <QScreen>

namespace GLOBAL {

const int FPS = 60;
const int TileLength = 32;
const int Scale = 2;
const int ObjectLength = Scale * TileLength;
const QSize ObjectSize = QSize(ObjectLength,ObjectLength);

}

#endif // GLOBAL_H
