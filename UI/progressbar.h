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

class ProgressBar : public QGraphicsRectItem, public GameObject
{
public:
    ProgressBar(QGraphicsItem* parent = 0);
    ~ProgressBar();

    virtual void update(int deltatime) override;

    void setMaximum(qreal maximum);
    void setValue(qreal value);
    void setProgressColour(QColor colour);
    void setProgressLowColour(QColor colour);
    void setBackgroundColour(QColor colour);
    void setOutlineColour(QColor colour);

    void setRect(qreal x, qreal y, qreal w, qreal h);
    void setRect(const QRectF &rect);

    void setLowPercent(qreal percent);

    void setDirection(Direction dir);
    void setAllignment(Allignment allignment);

    int getValue();
    int getMaximum();

private:
    int m_elapsedTime;
    qreal m_maximum;
    qreal m_currentVal;
    qreal m_changeInVal;
    qreal m_newVal;
    qreal m_lowPercent;

    QColor m_progressLowColour;
    QColor m_progressColour;

    QGraphicsRectItem* m_progressLine;
    QGraphicsRectItem* m_backgroundLine;

    Allignment m_allignment;
    Direction m_direction;

    qreal m_progressChangeRate;
};

#endif // PROGRESSBAR_H
