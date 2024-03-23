#ifndef BUTTONMENU_H
#define BUTTONMENU_H

#include "button.h"

class ButtonMenu
{
public:
    ButtonMenu();
    void update(KeyMap* keys);
    void addButton(Button* button);
    void removeButton(int index, QGraphicsScene &scene);
    void removeButton(Button* button, QGraphicsScene &scene);
    void render(QGraphicsScene &scene);
    void removeFromScene(QGraphicsScene &scene);
    void setPos(qreal x, qreal y);
    void setVertical();
    void setHorizontal();
    QSize rectSize();
    int numberOfButtons();
    bool buttonReleased();
    bool isRendered();
    int focusedButtonIndex();
    bool contains(Button* button);

private:
    QList<Button*> m_buttons;
    QSize m_rectSize;
    bool m_vertical;
    bool m_rendered;
    float m_x;
    float m_y;
    int m_focusedIndex;
};

#endif // BUTTONMENU_H
