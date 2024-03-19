#include "button.h"

#include <QFont>
#include <QPixmap>

#include "global.h"

Button::Button(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent)
{
    m_textBox = new QGraphicsTextItem;
    m_textBox->setDefaultTextColor(Qt::white);
    QFont font("Mono", GLOBAL::FONT_SIZE);
    font.setBold(true);
    m_textBox->setFont(font);

    QPixmap buttonPixmap;
    buttonPixmap.load(":/image/UI/res/Button.png");
    setPixmap(buttonPixmap.scaled(buttonPixmap.width() * GLOBAL::Scale,
                                  buttonPixmap.height() * GLOBAL::Scale));
    m_textBox->setTextWidth(boundingRect().width()/2);

    m_clicked = false;
}

void Button::update(KeyMap * keys)
{
    if (isUnderMouse() && keys->mouseHeldStatus() && keys->mouseFramesHeld() == 1)
    {
        m_clicked = true;
    }
    else if (m_clicked && !keys->mouseHeldStatus())
    {
        m_clicked = false;
    }
}

void Button::reset()
{
    m_clicked = false;
}

void Button::setText(QString text)
{
    m_textBox->setPlainText(text);
    m_textBox->setZValue(GLOBAL::UI_LAYER + 1);
}

void Button::setPos(qreal x, qreal y)
{
    QGraphicsPixmapItem::setPos(x, y);
    m_textBox->setPos(x + 2 * GLOBAL::Scale, y + boundingRect().height()/2 - m_textBox->boundingRect().height()/2);
}

void Button::render(QGraphicsScene &scene)
{
    scene.addItem(this);
    scene.addItem(m_textBox);
}

void Button::removeFromScene(QGraphicsScene &scene)
{
    scene.removeItem(this);
    scene.removeItem(m_textBox);
}

bool Button::isClicked()
{
    return m_clicked;
}
