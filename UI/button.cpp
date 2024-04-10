#include "button.h"

#include <QFont>

#include "../Utils/global.h"

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
    m_mouseMode = true;
    m_released = false;
    m_active = false;
    m_paused = false;
}

void Button::update(KeyMap * keys)
{
    m_released = false;

    // Check if focused
    if (m_mouseMode && isUnderMouse())
    {
        m_focused = true;
    }

    // Check if clicked
    if ((m_focused && ((m_mouseMode && keys->mouseHeldStatus() && keys->mouseFramesHeld() == 1) ||
                       (keys->keyHeldStatus(GLOBAL::SELECT)))))
    {
        m_clicked = true;
    }
    // Check if keys are released
    if (m_clicked && ((m_mouseMode && keys->mouseReleasedStatus() && isUnderMouse()) ||
                      keys->keyReleasedStatus(GLOBAL::SELECT)))
    {
        m_released = true;
        m_clicked = false;
    }
    else if (m_clicked && (m_mouseMode && keys->mouseReleasedStatus() && !isUnderMouse()))
    {
        m_clicked = false;
    }

    // Render according to status
    if (m_clicked)
    {
        setPixmap(m_clickedPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                             m_defaultPixmap.height() * GLOBAL::Scale));
    }
    else if (m_focused)
    {
        if (!m_iconText->isVisible())   m_iconText->show();
        setPixmap(m_focusedPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                       m_defaultPixmap.height() * GLOBAL::Scale));
    }
    else
    {
        if (m_iconText->isVisible())    m_iconText->hide();
        setPixmap(m_defaultPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                         m_defaultPixmap.height() * GLOBAL::Scale));
    }
}

void Button::reset()
{
    m_clicked = false;
    m_released = false;
}

void Button::setText(QString text)
{
    m_textBox->setPlainText(text);
}

void Button::setIconText(QString text)
{
    m_iconText->setPlainText(text);
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

void Button::setActive(bool value)
{
    m_active = value;
}

void Button::pause()
{
    m_paused = true;
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

void Button::removeItem(QGraphicsScene &scene)
{
    scene.removeItem(this);
    scene.removeItem(m_textBox);
    scene.removeItem(m_iconText);
}

void Button::hide()
{
    QGraphicsItem::hide();
    m_textBox->hide();
    m_iconText->hide();
}

void Button::show()
{
    QGraphicsItem::show();
    m_textBox->show();
    m_iconText->show();
}

bool Button::isTriggered()
{
    return m_released;
}

bool Button::isActive()
{
    return m_active;
}

bool Button::isPaused()
{
    return m_paused;
}
