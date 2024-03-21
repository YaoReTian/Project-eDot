#include "button.h"

#include <QFont>

#include "global.h"

Button::Button(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent)
{
    m_textBox = new QGraphicsTextItem;
    m_textBox->setDefaultTextColor(Qt::white);
    QFont font("Mono", GLOBAL::FONT_SIZE);
    font.setBold(true);
    m_textBox->setFont(font);

    m_defaultPixmap.load(":/image/UI/res/PopupButtonDefault.png");
    m_clickedPixmap.load(":/image/UI/res/PopupButtonClicked.png");
    m_hoverPixmap.load(":/image/UI/res/PopupButtonHover.png");

    setPixmap(m_defaultPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                     m_defaultPixmap.height() * GLOBAL::Scale));
    m_textBox->setTextWidth(boundingRect().width()/2);

    m_clicked = false;
}

void Button::update(int deltatime, KeyMap * keys)
{
    // Check clicked
    if (isUnderMouse() && keys->mouseHeldStatus() && keys->mouseFramesHeld() == 1)
    {
        m_clicked = true;
    }
    else if (m_clicked && !keys->mouseHeldStatus())
    {
        m_clicked = false;
        m_elapsedTime = 0;
    }
    else if (m_clicked && keys->mouseHeldStatus())
    {
        m_elapsedTime += deltatime;
    }

    // Render according to status
    if (m_clicked)
    {
        setPixmap(m_clickedPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                             m_defaultPixmap.height() * GLOBAL::Scale));
    }
    else if (isUnderMouse())
    {
        setPixmap(m_hoverPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                       m_defaultPixmap.height() * GLOBAL::Scale));
    }
    else if (!isUnderMouse())
    {
        setPixmap(m_defaultPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                         m_defaultPixmap.height() * GLOBAL::Scale));
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
    setZValue(GLOBAL::UI_LAYER + y);
    m_textBox->setZValue(zValue() + 1);
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
