#include "button.h"

#include <QFont>

#include "../Utils/global.h"
Button::Button(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent), m_elapsedTime(0), m_clicked(false), m_released(false),
    m_focused(false), m_mouseMode(true), m_active(false), m_paused(false), m_trigger(GLOBAL::SELECT),
    m_defaultPixmap(":/image/UI/res/PopupButtonDefault.png"),
    m_focusedPixmap(":/image/UI/res/PopupButtonFocused.png"),
    m_clickedPixmap(":/image/UI/res/PopupButtonClicked.png"),
    m_triggerValue(0)
{
    m_textBox = new QGraphicsTextItem(this);
    m_iconText = new QGraphicsTextItem(this);
    m_iconText->setPlainText("F");
    m_textBox->setDefaultTextColor(Qt::white);
    m_iconText->setDefaultTextColor(Qt::white);
    QFont font("Mono", 5 * GLOBAL::Scale);
    font.setBold(true);
    m_textBox->setFont(font);
    font.setPointSize(8 * GLOBAL::Scale);
    m_iconText->setFont(font);

    setPixmap(m_defaultPixmap.scaled(m_defaultPixmap.width() * GLOBAL::Scale,
                                     m_defaultPixmap.height() * GLOBAL::Scale));
    m_textBox->setTextWidth(boundingRect().width()/2);

    m_textBox->setPos(30 * GLOBAL::Scale, boundingRect().height()/2 - m_textBox->boundingRect().height()/2);
    m_iconText->setPos(9 * GLOBAL::Scale,
                       boundingRect().height()/2 - m_iconText->boundingRect().height()/2);
}

Button::~Button()
{
    delete m_textBox;
    delete m_iconText;
}

void Button::input(KeyMap* keys)
{
    m_released = false;

    // Check if focused
    if (m_mouseMode && isUnderMouse())
    {
        m_focused = true;
    }

    // Check if clicked
    if ((m_focused && ((isUnderMouse() && m_mouseMode && keys->mouseHeldStatus() && keys->mouseFramesHeld() == 1) ||
                       (keys->keyHeldStatus(m_trigger)))))
    {
        m_clicked = true;
    }

    // Check if keys are released
    if (m_clicked && ((m_mouseMode && keys->mouseReleasedStatus() && isUnderMouse()) ||
                      keys->keyReleasedStatus(m_trigger)))
    {
        m_released = true;
        m_clicked = false;
        emit triggered(m_triggerValue);
    }
    else if (m_clicked && (m_mouseMode && keys->mouseReleasedStatus() && !isUnderMouse()))
    {
        m_clicked = false;
    }
}

void Button::update(int deltatime)
{
    setZValue(m_baseZ + y());
    if (m_released && m_elapsedTime < 40)
    {
        m_released = false;
    }

    if (m_clicked)
    {
        m_elapsedTime += deltatime;
    }
    else
    {
        m_elapsedTime = 0;
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

void Button::addTriggerValue(int n)
{
    m_triggerValue = n;
}

void Button::hide()
{
    m_clicked = false;
    m_released = false;
    m_elapsedTime = 0;
    m_focused = false;
    m_mouseMode = true;
    m_active = false;
    m_trigger = GLOBAL::SELECT;
    m_textBox->setPlainText("");
    m_iconText->setPlainText("F");
    m_triggerValue = 0;
    QGraphicsPixmapItem::hide();
}

void Button::setText(QString text)
{
    m_textBox->setPlainText(text);
}

void Button::setIconText(QString text)
{
    m_iconText->setPlainText(text);
}

void Button::setMousemode(bool value)
{
    m_mouseMode = value;
}

void Button::setZValue(qreal z)
{
    m_baseZ = z;
}

void Button::setFocused()
{
    m_focused = true;
}

void Button::setActive(bool value)
{
    m_active = value;
}

void Button::setTriggerAction(GLOBAL::Action action)
{
    m_trigger = action;
}

void Button::pause()
{
    m_paused = true;
}

void Button::removeFocus()
{
    m_focused = false;
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
