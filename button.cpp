#include "button.h"

#include <QFont>

#include "global.h"

Button::Button(QGraphicsItem* parent) : QObject(), QGraphicsPixmapItem(parent)
{
    m_textBox = new QGraphicsTextItem;
    m_iconText = new QGraphicsTextItem;
    m_textBox->setDefaultTextColor(Qt::white);
    m_iconText->setDefaultTextColor(Qt::white);
    QFont font("Mono", 5 * GLOBAL::Scale);
    font.setBold(true);
    m_textBox->setFont(font);
    font.setPointSize(8 * GLOBAL::Scale);
    m_iconText->setFont(font);
    m_iconText->setPlainText("F");

    m_defaultPixmap.load(":/image/UI/res/PopupButtonDefault.png");
    m_clickedPixmap.load(":/image/UI/res/PopupButtonClicked.png");
    m_focusedPixmap.load(":/image/UI/res/PopupButtonFocused.png");
    m_iconDefault.load(":/image/UI/res/PopupIconDefault.png");
    m_iconClicked.load(":/image/UI/res/PopupIconClicked.png");

    setPixmap(m_defaultPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                     m_defaultPixmap.height() * GLOBAL::Scale));
    m_textBox->setTextWidth(boundingRect().width()/2);

    m_clicked = false;
    m_focused = false;
}

void Button::update(int deltatime, KeyMap * keys)
{
    // Check clicked
    if ((isUnderMouse() && keys->mouseHeldStatus() && keys->mouseFramesHeld() == 1) ||
        (keys->keyHeldStatus(GLOBAL::SELECT) && m_focused))
    {
        m_clicked = true;
    }
    // Check clicked but then let go
    else if (m_clicked && !keys->mouseHeldStatus())
    {
        m_clicked = false;
        m_elapsedTime = 0;
    }
    // Check if button is held
    else if (m_clicked && keys->mouseHeldStatus())
    {
        m_elapsedTime += deltatime;
    }
    // Check if focused
    else if (isUnderMouse())
    {
        m_focused = true;
    }

    // Render according to status
    if (m_clicked)
    {
        setPixmap(m_clickedPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                             m_defaultPixmap.height() * GLOBAL::Scale));
    }
    else if (m_focused)
    {
        setPixmap(m_focusedPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                       m_defaultPixmap.height() * GLOBAL::Scale));
    }
    else
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
}

void Button::setPos(qreal x, qreal y)
{
    QGraphicsPixmapItem::setPos(x, y);
    m_textBox->setPos(x + 30 * GLOBAL::Scale, y + boundingRect().height()/2 - m_textBox->boundingRect().height()/2);
    m_iconText->setPos(x + 9 * GLOBAL::Scale,
                       y + boundingRect().height()/2 - m_iconText->boundingRect().height()/2);
    setZValue(GLOBAL::UI_LAYER + y);
    m_textBox->setZValue(zValue() + 1);
    m_iconText->setZValue(zValue() + 1);
}

void Button::setFocused()
{
    m_focused = true;
}

void Button::removeFocus()
{
    m_focused = false;
}

void Button::render(QGraphicsScene &scene)
{
    scene.addItem(this);
    scene.addItem(m_textBox);
    scene.addItem(m_iconText);
}

void Button::removeFromScene(QGraphicsScene &scene)
{
    scene.removeItem(this);
    scene.removeItem(m_textBox);
    scene.removeItem(m_iconText);
}

bool Button::isClicked()
{
    return m_clicked;
}
