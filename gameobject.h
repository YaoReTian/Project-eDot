#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsScene>

class GameObject
{
public:
    virtual void update(int deltatime) = 0;

    virtual ~GameObject() {};
};

#endif // GAMEOBJECT_H
