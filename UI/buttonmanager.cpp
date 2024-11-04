#include "buttonmanager.h"

ButtonManager::ButtonManager(QGraphicsItem *parent)
{
    m_parent = parent;
    for (int i = 0; i < 5; i++)
    {
        Button* b = new Button(parent);
        b->hide();
        m_pool.enqueue(b);
    }
}

ButtonManager::~ButtonManager()
{
    clear();
    qDeleteAll(m_pool);
    m_pool.clear();
}

void ButtonManager::setBaseZ(qreal z)
{
    m_baseZ = z;
}

void ButtonManager::setPos(QPointF pos)
{
    m_pos = pos;
}

void ButtonManager::setPos(qreal x, qreal y)
{
    m_pos.setX(x);
    m_pos.setY(y);
}

qreal ButtonManager::width()
{
    if (m_activeButtons.empty())
    {
        return 0;
    }

    return m_activeButtons[0]->boundingRect().width();
}

qreal ButtonManager::height()
{
    if (m_activeButtons.empty())
    {
        return 0;
    }

    return m_activeButtons.size() * (m_activeButtons[0]->boundingRect().height() + 2*GLOBAL::Scale) - 2*GLOBAL::Scale;
}

Button* ButtonManager::getButtonFromPool()
{
    Button* b = m_pool.dequeue();
    b->setParentItem(m_parent);
    b->setActive(true);
    b->show();
    return b;
}

void ButtonManager::addButton(Button *b)
{
    m_activeButtons.append(b);
}

void ButtonManager::clear()
{
    m_focusedIndex = 0;
    while (!m_activeButtons.empty())
    {
        m_activeButtons.back()->disconnect();
        m_activeButtons.back()->hide();
        m_pool.enqueue(m_activeButtons.takeLast());
    }
}

void ButtonManager::input(KeyMap* keys)
{
    if (!m_activeButtons.empty())
    {
        if (keys->keyHeldStatus(GLOBAL::NEXT_OPTION) && m_elapsedTime >= 200)
        {
            m_activeButtons[m_focusedIndex]->removeFocus();
            m_focusedIndex++;
            if (m_focusedIndex == m_activeButtons.size())  m_focusedIndex = 0;
            m_elapsedTime = 0;
        }
        m_activeButtons[m_focusedIndex]->setFocused();
        m_activeButtons[m_focusedIndex]->input(keys);
    }
}

void ButtonManager::update(int deltatime)
{
    m_elapsedTime += deltatime;
    int i = 0;
    int size = m_activeButtons.size();
    while (i < size && !m_activeButtons.empty())
    {
        m_activeButtons[i]->setPos(m_pos.x(),
                                   m_pos.y() + i * (m_activeButtons[i]->boundingRect().height() + 2 * GLOBAL::Scale));
        m_activeButtons[i]->update(deltatime);
        i++;
    }
}

void ButtonManager::setParentItem(QGraphicsItem *parent)
{
    m_parent = parent;
}
