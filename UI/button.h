#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPixmap>
#include "../Utils/keymap.h"

class Button : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Button(QGraphicsItem* parent = 0);

    void update(KeyMap * keys);
    void reset();
    void setText(QString text);
    void render(QGraphicsScene &scene);
    void removeFromScene(QGraphicsScene &scene);
    void setPos(qreal x, qreal y);
    void setFocused();
    void removeFocus();
    void setIconText(QString text);
    void pause();

    bool isTriggered();
    bool isRendered();
    bool isPaused();

private:
    QGraphicsTextItem* m_textBox;
    QGraphicsTextItem* m_iconText;
    bool m_clicked;
    bool m_released;
    bool m_focused;
    bool m_mouseMode;
    bool m_rendered;
    bool m_paused;

    QPixmap m_defaultPixmap;
    QPixmap m_focusedPixmap;
    QPixmap m_clickedPixmap;

    //Icon only appears when focused
    QPixmap m_iconDefault;
    QPixmap m_iconClicked;
};

#endif // BUTTON_H
