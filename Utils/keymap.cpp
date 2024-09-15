#include "keymap.h"

#include <QtGlobal>

KeyMap::KeyMap()
{
    for (int n = GLOBAL::MOVE_LEFT; n != GLOBAL::NONE; n++)
    {
        m_keyStatus[static_cast<GLOBAL::Action>(n)] = new KeyStatus;
    }
    m_mouseStatus = new MouseStatus;
}

KeyMap::~KeyMap()
{
    qDeleteAll(m_keyStatus);
    m_keyStatus.clear();
    delete m_mouseStatus;
}

void KeyMap::setWorldBindings()
{
    m_keys.clear();
    m_keys[Qt::Key_A]       = GLOBAL::MOVE_LEFT;
    m_keys[Qt::Key_D]       = GLOBAL::MOVE_RIGHT;
    m_keys[Qt::Key_W]       = GLOBAL::MOVE_UP;
    m_keys[Qt::Key_S]       = GLOBAL::MOVE_DOWN;
    m_keys[Qt::Key_Q]       = GLOBAL::SPRINT;
    m_keys[Qt::Key_F]       = GLOBAL::SELECT;
    m_keys[Qt::Key_Down]    = GLOBAL::NEXT_OPTION;
    m_keys[Qt::Key_Up]      = GLOBAL::PREV_OPTION;
    m_keys[Qt::Key_R]       = GLOBAL::SHOW_HITBOX;
}

void KeyMap::setTurnBasedBindings()
{
    m_keys.clear();
    m_keys[Qt::Key_1]       = GLOBAL::OVERDRIVE_1;
    m_keys[Qt::Key_2]       = GLOBAL::OVERDRIVE_2;
    m_keys[Qt::Key_3]       = GLOBAL::OVERDRIVE_3;
    m_keys[Qt::Key_4]       = GLOBAL::OVERDRIVE_4;
    m_keys[Qt::Key_F]       = GLOBAL::SELECT;
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

void KeyMap::resetStatus()
{
    for (auto [key, value] : m_keyStatus.asKeyValueRange())
    {
        value->m_released = false;
    }

    if (m_mouseStatus->m_held)
    {
        m_mouseStatus->m_framesHeld++;
    }
    else
    {
        m_mouseStatus->m_framesHeld = 0;
    }
    m_mouseStatus->m_released = false;
}

void KeyMap::keyHeld(GLOBAL::Action action)
{
    m_keyStatus[action]->m_held = true;
}

void KeyMap::keyReleased(GLOBAL::Action action)
{
    m_keyStatus[action]->m_released = true;
    m_keyStatus[action]->m_held = false;
}

bool KeyMap::keyHeldStatus(GLOBAL::Action action)
{
    return m_keyStatus[action]->m_held;
}

bool KeyMap::keyReleasedStatus(GLOBAL::Action action)
{
    return m_keyStatus[action]->m_released;
}

void KeyMap::mouseHeld()
{
    m_mouseStatus->m_held= true;
}

void KeyMap::mouseReleased()
{
    m_mouseStatus->m_held = false;
    m_mouseStatus->m_released = true;
}

bool KeyMap::mouseHeldStatus()
{
    return m_mouseStatus->m_held;
}

bool KeyMap::mouseReleasedStatus()
{
    return m_mouseStatus->m_released;
}

int KeyMap::mouseFramesHeld()
{
    return m_mouseStatus->m_framesHeld;
}
