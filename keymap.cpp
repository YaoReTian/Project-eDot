#include "keymap.h"

#include <QtGlobal>

KeyMap::KeyMap()
{
    for (int n = GLOBAL::MOVE_LEFT; n != GLOBAL::NONE; n++)
    {
        m_keyStatus[static_cast<GLOBAL::Action>(n)] = new KeyStatus;
    }
}

void KeyMap::setDefaultBindings()
{
    m_keys[Qt::Key_A]       = GLOBAL::MOVE_LEFT;
    m_keys[Qt::Key_Left]    = GLOBAL::MOVE_LEFT;
    m_keys[Qt::Key_D]       = GLOBAL::MOVE_RIGHT;
    m_keys[Qt::Key_Right]   = GLOBAL::MOVE_RIGHT;
    m_keys[Qt::Key_W]       = GLOBAL::MOVE_UP;
    m_keys[Qt::Key_Up]      = GLOBAL::MOVE_UP;
    m_keys[Qt::Key_S]       = GLOBAL::MOVE_DOWN;
    m_keys[Qt::Key_Down]    = GLOBAL::MOVE_DOWN;
}

void KeyMap::setBinding(Qt::Key key, GLOBAL::Action action)
{
    m_keys[key] = action;
}

GLOBAL::Action KeyMap::getAction(Qt::Key key)
{
    return m_keys[key];
}

bool KeyMap::contains(Qt::Key key)
{
    return m_keys.contains(key);
}

void KeyMap::resetKeyStatus()
{
    for (auto [key, value] : m_keyStatus.asKeyValueRange())
    {
        value->m_released = false;
    }
}

void KeyMap::setHeld(GLOBAL::Action action, bool value)
{
    m_keyStatus[action]->m_held = value;
}

void KeyMap::setReleased(GLOBAL::Action action, bool value)
{
    m_keyStatus[action]->m_released = value;
}

bool KeyMap::getHeldStatus(GLOBAL::Action action)
{
    return m_keyStatus[action]->m_held;
}

bool KeyMap::getReleasedStatus(GLOBAL::Action action)
{
    return m_keyStatus[action]->m_released;
}
