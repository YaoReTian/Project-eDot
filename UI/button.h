#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPixmap>
#include "../Utils/keymap.h"
#include "../gameobject.h"

class Button : public QObject, public QGraphicsPixmapItem, public GameObject
{
    Q_OBJECT
public:
    Button(QGraphicsItem* parent = 0);
    ~Button();

    void input(KeyMap* keys);

    virtual void clear(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void reset();
    void setText(QString text);
    void setPos(qreal x, qreal y);
    void setFocused();
    void setActive(bool value);
    void setTriggerAction(GLOBAL::Action);
    void removeFocus();
    void setIconText(QString text);
    void setMousemode(bool value);
    void pause();
    void hide();
    void show();

    bool isTriggered();
    bool isActive();
    bool isPaused();

private:
    int m_elapsedTime;

    QGraphicsTextItem* m_textBox;
    QGraphicsTextItem* m_iconText;
    bool m_clicked;
    bool m_released;
    bool m_focused;
    bool m_mouseMode;
    bool m_active;
    bool m_paused;
    GLOBAL::Action m_trigger;

    QPixmap m_defaultPixmap;
    QPixmap m_focusedPixmap;
    QPixmap m_clickedPixmap;
};

#endif // BUTTON_H
