#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPixmap>
#include "../Utils/keymap.h"
#include "../gameobject.h"
#include "../database.h"

class Button : public QObject, public QGraphicsPixmapItem, public GameObject
{
    Q_OBJECT
public:
    Button(QGraphicsItem* parent = 0);
    ~Button();

    void input(KeyMap* keys);

    virtual void update(int deltatime) override;

    void hide();
    void hideStandard();
    void setText(QString text);
    void setZValue(qreal z);
    void setFocused();
    void setActive(bool value);
    void setTriggerAction(GLOBAL::Action action);
    void removeFocus();
    void setIconText(QString text);
    void setMousemode(bool value);
    void pause();

    QString text() const;

    bool isTriggered();
    bool isActive();
    bool isPaused();

    void addTriggerValue(int n);

signals:
    void triggered(int n = 0);

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

    qreal m_baseZ;
    int m_triggerValue;
};

#endif // BUTTON_H
