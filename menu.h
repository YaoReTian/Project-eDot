#ifndef MENU_H
#define MENU_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsPixmapItem>
#include "gameobject.h"

class Menu : public QGraphicsRectItem, public GameObject
{
public:
    Menu(QGraphicsItem* parent = 0);
    ~Menu();

    virtual void update(int deltatime) override;
    void setMessage(QString message);
    void setText(QPixmap pixmap);

private:
    QGraphicsTextItem* m_message;
    QGraphicsPixmapItem* m_text;
};

#endif // MENU_H
