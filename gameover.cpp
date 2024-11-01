#include "gameover.h"

GameOver::GameOver(QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_text(new QGraphicsPixmapItem)
{
    hide();
    QColor c(Qt::red);
    c.setAlphaF(0.5);
    setBrush(c);
    m_text->setParentItem(this);
    m_text->setPixmap(QPixmap(":/image/res/Game-over.png"));
}

GameOver::~GameOver()
{
    delete m_text;
}

void GameOver::update(int deltatime)
{
    m_text->setPos(boundingRect().width()/2 - m_text->boundingRect().width()/2,
                   boundingRect().height()/2 - m_text->boundingRect().height()/2 - 30);
}
