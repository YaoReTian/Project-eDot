#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QGraphicsScene>

class GameObject
{
public:
    virtual void removeItem(QGraphicsScene &scene) = 0;
    virtual void update(int deltatime) = 0;
    virtual void render(QGraphicsScene &scene) = 0;

    virtual ~GameObject() {};
};

#endif // GAMEOBJECT_H
