#include "buttonmenu.h"

ButtonMenu::ButtonMenu()
    : m_rectSize(0,0), m_vertical(true), m_active(false), m_x(0), m_y(0),
    m_focusedIndex(0), m_elapsedTime(0), m_numberOfButtons(0)
{
}

ButtonMenu::~ButtonMenu()
{
    qDeleteAll(m_buttons);
    m_buttons.clear();
    qDeleteAll(m_newButtons);
    m_newButtons.clear();
}

void ButtonMenu::input(KeyMap* keys)
{
    if (!m_buttons.empty())
    {
        if (keys->keyHeldStatus(GLOBAL::NEXT_OPTION) && m_elapsedTime >= 100)
        {
            m_buttons[m_focusedIndex]->removeFocus();
            m_focusedIndex++;
            if (m_focusedIndex == m_buttons.size())  m_focusedIndex = 0;
            m_elapsedTime = 0;
        }
        else if (keys->keyHeldStatus(GLOBAL::PREV_OPTION) && m_elapsedTime > 100)
        {
            m_buttons[m_focusedIndex]->removeFocus();
            m_focusedIndex--;
            if (m_focusedIndex == -1)  m_focusedIndex = m_buttons.size()-1;
            m_elapsedTime = 0;
        }
        m_buttons[m_focusedIndex]->setFocused();
        m_buttons[m_focusedIndex]->input(keys);
    }
}

void ButtonMenu::update(int deltatime)
{
    m_elapsedTime += deltatime;
    if (!m_buttons.empty())
    {
        for (auto b : m_buttons)
        {
            b->update(deltatime);
        }
    }
}

void ButtonMenu::addButton(Button* button)
{
    if (m_buttons.size() == 0)  m_active = true;
    m_numberOfButtons++;
    if (m_vertical)
    {
        m_rectSize.setHeight(m_rectSize.height() + button->boundingRect().height() + 2*GLOBAL::Scale);
    }
    else
    {
        m_rectSize.setWidth(m_rectSize.width() + button->boundingRect().width() + 2*GLOBAL::Scale);
    }
    m_newButtons.enqueue(button);
}

void ButtonMenu::removeButton(int index)
{
    if (m_vertical)
    {
        for (int i = index; i < m_numberOfButtons; i++)
        {
            m_buttons[i]->setPos(m_x,
                                 m_y + i*(m_buttons[i]->boundingRect().height() +  2*GLOBAL::Scale));
        }
        m_rectSize.setHeight(m_rectSize.height() - (m_buttons[index]->boundingRect().height() + 2*GLOBAL::Scale));
    }
    else
    {
        for (int i = index; i < m_numberOfButtons; i++)
        {
            m_buttons[i]->setPos(m_x - i*(m_buttons[i]->boundingRect().width() + 2*GLOBAL::Scale),
                                 m_y);
        }
        m_rectSize.setWidth(m_rectSize.width() - (m_buttons[index]->boundingRect().width() + 2*GLOBAL::Scale));
    }
    m_buttons[index]->removeFocus();

    if (m_focusedIndex == m_numberOfButtons)
    {
        m_focusedIndex = 0;
    }
    m_numberOfButtons--;
    if (m_numberOfButtons == 0)  m_active = false;
}

void ButtonMenu::removeButton(Button * button)
{
    if (m_buttons.indexOf(button) != -1)
    {
        removeButton(m_buttons.indexOf(button));
    }
    else
    {
        qDebug() << "Button not found";
    }
}

void ButtonMenu::setPos(qreal x, qreal y)
{
    m_x = x;
    m_y = y;
    for (int i = 0; i < m_buttons.size(); i++)
    {
        if (m_vertical)
        {
            m_buttons[i]->setPos(x, y + i * (m_buttons[i]->boundingRect().height() + 2 * GLOBAL::Scale));
        }
        else
        {
            m_buttons[i]->setPos(x + i * (m_buttons[i]->boundingRect().width() + 2 * GLOBAL::Scale), y);
        }
    }
}

void ButtonMenu::setVertical()
{
    m_vertical = true;
}

void ButtonMenu::setHorizontal()
{
    m_vertical = false;
}

void ButtonMenu::setActive(bool status)
{
    m_active = status;
}

QSize ButtonMenu::rectSize()
{
    return m_rectSize;
}

int ButtonMenu::numberOfButtons()
{
    return m_numberOfButtons;
}

bool ButtonMenu::buttonReleased()
{
    if (!m_buttons.empty())
    {
        return m_buttons[m_focusedIndex]->isTriggered();
    }
    else
    {
        return false;
    }
}

bool ButtonMenu::isActive()
{
    return m_active;
}

int ButtonMenu::focusedButtonIndex()
{
    return m_focusedIndex;
}

bool ButtonMenu::contains(Button* button)
{
    return m_buttons.contains(button);
}

