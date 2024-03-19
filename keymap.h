#ifndef KEYMAP_H
#define KEYMAP_H

#include <QMap>
#include <QtGlobal>
#include "global.h"

struct KeyStatus
{
    bool m_held = false;
    bool m_released = false;
};

struct MouseStatus
{
    bool m_held = false;
    bool m_released = false;
    int m_framesHeld = 0;
};

class KeyMap
{
public:
    KeyMap();
    void setDefaultBindings();
    void setBinding(Qt::Key key, GLOBAL::Action action);
    GLOBAL::Action getAction(Qt::Key);
    bool contains(Qt::Key key);

    void resetStatus();
    void keyHeld(GLOBAL::Action action);
    void keyReleased(GLOBAL::Action action);
    bool keyHeldStatus(GLOBAL::Action action);
    bool keyReleasedStatus(GLOBAL::Action action);

    void mouseHeld();
    void mouseReleased();
    bool mouseHeldStatus();
    bool mouseReleasedStatus();
    int mouseFramesHeld();

private:
    QMap<Qt::Key, GLOBAL::Action> m_keys;
    QMap<GLOBAL::Action, KeyStatus*> m_keyStatus;
    MouseStatus* m_mouseStatus;
};

#endif // KEYMAP_H
