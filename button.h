#ifndef BUTTON_H
#define BUTTON_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QPixmap>
#include "keymap.h"

class Button : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Button(QGraphicsItem* parent = 0);

    void update(int deltatime, KeyMap * keys);
    void reset();
    void setText(QString text);
    void render(QGraphicsScene &scene);
    void removeFromScene(QGraphicsScene &scene);
    void setPos(qreal x, qreal y);

    bool isClicked();

private:
    QGraphicsTextItem* m_textBox;
    bool m_clicked;

    int m_elapsedTime;
    QPixmap m_defaultPixmap;
    QPixmap m_hoverPixmap;
    QPixmap m_clickedPixmap;
};

#endif // BUTTON_H
