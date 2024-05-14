#ifndef DOUBLEBUFFER_H
#define DOUBLEBUFFER_H

#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QPixmap>
#include <QColor>

class DoubleBuffer : public QGraphicsPixmapItem
{
public:
    DoubleBuffer(QGraphicsItem * parent = 0);
    ~DoubleBuffer();

    void swap();
    void clear();
    void setBackground(QColor colour);
    void setBackground(QPixmap pixmap);
    void setBackground(QList<QList<QPixmap>> list);
    void drawItem(QPixmap pixmap, float a_x, float a_y);

private:

    QPainter m_painter;
    QPixmap* m_buffer[2];
};

#endif // DOUBLEBUFFER_H
