#include "card.h"

Card::Card()
{

}

Card::~Card()
{

}

void Card::removeItem(QGraphicsScene &scene)
{
    Sprite::removeItem(scene);
}

void Card::update(int deltatime)
{
    Sprite::update(deltatime);
}

void Card::render(QGraphicsScene &scene)
{
    Sprite::render(scene);
}
