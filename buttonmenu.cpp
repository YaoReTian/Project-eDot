#include "buttonmenu.h"

ButtonMenu::ButtonMenu()
{
    m_rectSize.setHeight(0);
    m_rectSize.setWidth(0);
    m_vertical = true;
    m_focusedIndex = 0;
}

void ButtonMenu::update(KeyMap* keys)
{
    if (!m_buttons.empty())
    {
        if (keys->keyHeldStatus(GLOBAL::NEXT_OPTION))
        {
            m_buttons[m_focusedIndex]->removeFocus();
            m_focusedIndex++;
            if (m_focusedIndex == m_buttons.size())  m_focusedIndex = 0;
            m_buttons[m_focusedIndex]->setFocused();
        }
        for (auto b : m_buttons)
        {
            b->update(keys);
        }
    }
}

void ButtonMenu::addButton(Button* button)
{
    if (m_vertical)
    {
        button->setPos(m_x, m_rectSize.height() + 2 * GLOBAL::Scale);
        m_rectSize.setHeight(m_rectSize.height() + button->boundingRect().height() + 2*GLOBAL::Scale);
    }
    else
    {
        button->setPos(m_rectSize.width() + 2 * GLOBAL::Scale, m_y);
        m_rectSize.setWidth(m_rectSize.width() + button->boundingRect().width() + 2*GLOBAL::Scale);
    }
    m_buttons.append(button);
}

void ButtonMenu::removeButton(int index, QGraphicsScene &scene)
{
    if (m_vertical)
    {
        for (int i = index; i < m_buttons.size(); i++)
        {
            m_buttons[i]->setPos(m_buttons[i]->x(),
                                 m_buttons[i]->y() - m_buttons[i]->boundingRect().height() - 2*GLOBAL::Scale);
        }
        m_rectSize.setHeight(m_rectSize.height() - (m_buttons[index]->boundingRect().height() + 2*GLOBAL::Scale));
    }
    else
    {
        for (int i = index; i < m_buttons.size(); i++)
        {
            m_buttons[i]->setPos(m_buttons[i]->x() - m_buttons[i]->boundingRect().width(),
                                 m_buttons[i]->y());
        }
        m_rectSize.setWidth(m_rectSize.width() - (m_buttons[index]->boundingRect().width() + 2*GLOBAL::Scale));
    }
    m_buttons[index]->removeFromScene(scene);
    m_buttons.removeAt(index);
}

void ButtonMenu::removeButton(Button * button, QGraphicsScene &scene)
{
    if (m_buttons.indexOf(button) != -1)
    {
        removeButton(m_buttons.indexOf(button), scene);
    }
    else
    {
        qDebug() << "Button not found";
    }
}

void ButtonMenu::render(QGraphicsScene &scene)
{
    if (!m_buttons.empty())
    {
        m_rendered = true;
        m_focusedIndex = 0;
        m_buttons[m_focusedIndex]->setFocused();
        for (const auto b : m_buttons)
        {
            b->render(scene);
        }
    }
}

void ButtonMenu::removeFromScene(QGraphicsScene &scene)
{
    m_rendered = false;
    if (!m_buttons.empty())
    {
        for (const auto b : m_buttons)
        {
            b->removeFromScene(scene);
        }
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

QSize ButtonMenu::rectSize()
{
    return m_rectSize;
}

int ButtonMenu::numberOfButtons()
{
    return m_buttons.size();
}

bool ButtonMenu::buttonReleased()
{
    return m_buttons[m_focusedIndex]->isTriggered();
}

bool ButtonMenu::isRendered()
{
    return m_rendered;
}

int ButtonMenu::focusedButtonIndex()
{
    return m_focusedIndex;
}

bool ButtonMenu::contains(Button* button)
{
    return m_buttons.contains(button);
}

