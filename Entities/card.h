#ifndef CARD_H
#define CARD_H

#include "sprite.h"

class Card : public Sprite
{
public:
    Card();
    ~Card();

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

};

#endif // CARD_H
