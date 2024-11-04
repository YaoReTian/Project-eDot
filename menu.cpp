#include "menu.h"

#include "Utils/global.h"

Menu::Menu(QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_text(new QGraphicsPixmapItem), m_message(new QGraphicsTextItem)
{
    hide();
    m_text->setParentItem(this);
    m_message->setParentItem(this);
    m_message->setDefaultTextColor(Qt::white);
    QFont font("Mono", 10 * GLOBAL::Scale);
    font.setBold(true);
    font.setPointSize(8 * GLOBAL::Scale);
    m_message->setFont(font);
}

Menu::~Menu()
{
    delete m_text;
}

void Menu::update(int deltatime)
{
    if (m_message->textWidth() != 0)
    {
        m_message->setPos(boundingRect().width()/2 - m_message->boundingRect().width()/2,
                          boundingRect().height()/2 - m_message->boundingRect().height() + 3*GLOBAL::Scale);
    }
    m_text->setPos(boundingRect().width()/2 - m_text->boundingRect().width()/2,
                   boundingRect().height()/2 - m_text->boundingRect().height() + GLOBAL::Scale*4 - m_message->boundingRect().height());
}

void Menu::setMessage(QString message)
{
    m_message->setPlainText(message);
}

void Menu::setText(QPixmap pixmap)
{
    m_text->setPixmap(pixmap);
}
