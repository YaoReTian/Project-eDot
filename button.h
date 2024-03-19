#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include "keymap.h"

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

    bool isClicked();

private:
    QGraphicsTextItem* m_textBox;
    bool m_clicked;
};

#endif // BUTTON_H
