#ifndef BUTTONMENU_H
#define BUTTONMENU_H

#include "button.h"
#include <QQueue>

class ButtonMenu
{
public:
    ButtonMenu();

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime, KeyMap* keys);
    void render(QGraphicsScene &scene);

    void addButton(Button* button);
    void removeButton(int index);
    void removeButton(Button* button);

    void setPos(qreal x, qreal y);
    void setVertical();
    void setHorizontal();
    void setActive(bool status);

    bool isActive();

    QSize rectSize();
    int numberOfButtons();
    bool buttonReleased();
    int focusedButtonIndex();
    bool contains(Button* button);

private:
    QList<Button*> m_buttons;
    QQueue<Button*> m_newButtons;
    QSize m_rectSize;
    bool m_vertical;
    bool m_active;
    float m_x;
    float m_y;
    int m_focusedIndex;
    int m_elapsedTime;
    int m_numberOfButtons;
};

#endif // BUTTONMENU_H
