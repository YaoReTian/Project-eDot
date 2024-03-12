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

class KeyMap
{
public:
    KeyMap();
    void setDefaultBindings();
    void setBinding(Qt::Key key, GLOBAL::Action action);
    GLOBAL::Action getAction(Qt::Key);
    bool contains(Qt::Key key);

    void resetKeyStatus();
    void setHeld(GLOBAL::Action action, bool value);
    void setReleased(GLOBAL::Action action, bool value);
    bool getHeldStatus(GLOBAL::Action action);
    bool getReleasedStatus(GLOBAL::Action action);

private:
    QMap<Qt::Key, GLOBAL::Action> m_keys;
    QMap<GLOBAL::Action, KeyStatus*> m_keyStatus;
};

#endif // KEYMAP_H
