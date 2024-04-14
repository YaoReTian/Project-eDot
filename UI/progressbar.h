#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include "../gameobject.h"

enum Allignment
{
    Vertical,
    Horizontal
};

enum Direction
{
    BottomToTop,
    TopToBottom
};

class ProgressBar : public GameObject
{
public:
    ProgressBar();
    ~ProgressBar();

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    void setMaximum(qreal maximum);
    void setValue(qreal value);
    void setProgressColour(QColor colour);
    void setProgressLowColour(QColor colour);
    void setBackgroundColour(QColor colour);
    void setOutlineColour(QColor colour);
    void setPos(qreal x, qreal y);
    void setZValue(qreal a_zValue);

    void setLowPercent(qreal percent);
    void setWidth(int width);
    void setLength(int length);

    void setDirection(Direction dir);
    void setAllignment(Allignment allignment);

    int getValue();
    int getMaximum();
    qreal getWidth();
    qreal getLength();
    qreal x();
    qreal y();
    qreal zValue();
    QRectF boundingRect();

private:
    int m_elapsedTime;
    qreal m_maximum;
    qreal m_currentVal;
    qreal m_changeInVal;
    qreal m_newVal;
    qreal m_lowPercent;
    qreal m_x;
    qreal m_y;
    qreal m_zValue;

    qreal m_width;
    qreal m_length;

    QColor m_progressLowColour;
    QColor m_progressColour;

    QGraphicsRectItem* m_progressLine;
    QGraphicsRectItem* m_backgroundLine;
    QGraphicsRectItem* m_outline;

    Allignment m_allignment;
    Direction m_direction;

    qreal m_progressChangeRate;
};

#endif // PROGRESSBAR_H
