#include "progressbar.h"

#include <QtMath>

#include "../Utils/global.h"

ProgressBar::ProgressBar(QGraphicsItem* parent)
    : QGraphicsRectItem(parent), m_elapsedTime(0), m_maximum(100), m_currentVal(0), m_changeInVal(0), m_newVal(0),
    m_lowPercent(20), m_progressLowColour(Qt::red), m_progressColour(Qt::green), m_allignment(Horizontal),
    m_direction(BottomToTop), m_progressChangeRate(1000.0f)
{
    m_backgroundLine = new QGraphicsRectItem(this);
    m_progressLine = new QGraphicsRectItem(m_backgroundLine);

    m_backgroundLine->setBrush(Qt::gray);
    setBrush(Qt::black);

    m_progressLine->setPen(Qt::NoPen);
    m_backgroundLine->setPen(Qt::NoPen);
    setPen(QPen(Qt::white, 0.3f));
}

ProgressBar::~ProgressBar()
{
    delete m_progressLine;
    delete m_backgroundLine;
}

void ProgressBar::update(int deltatime)
{
    if (m_newVal != m_currentVal)
    {
        if (m_elapsedTime > m_progressChangeRate)
        {
            m_currentVal = m_newVal;
            m_elapsedTime = 0;
        }
        else
        {
            m_elapsedTime += deltatime;
            m_currentVal += m_changeInVal*deltatime;
        }

    }

    switch (m_allignment)
    {
    case Vertical:
        switch (m_direction)
        {
        case BottomToTop:
            m_progressLine->setRect(-5*GLOBAL::Scale,
                                    m_backgroundLine->boundingRect().height()*(1 - m_currentVal/m_maximum),
                                    m_backgroundLine->boundingRect().width(),
                                    m_backgroundLine->boundingRect().height()*(m_currentVal/m_maximum));
            break;
        case TopToBottom:
            m_progressLine->setRect(-5*GLOBAL::Scale,
                                    0,
                                    m_backgroundLine->boundingRect().width(),
                                    m_backgroundLine->boundingRect().height()*(m_currentVal/m_maximum));
            break;
        }
        break;
    case Horizontal:
        switch (m_direction)
        {
        case BottomToTop:
            m_progressLine->setRect(0,
                                    -5*GLOBAL::Scale,
                                    m_backgroundLine->boundingRect().width()*(m_currentVal/m_maximum),
                                    m_backgroundLine->boundingRect().height());
            break;
        case TopToBottom:
            m_progressLine->setRect(boundingRect().width()*(1-m_currentVal/m_maximum),
                                    -5*GLOBAL::Scale,
                                    m_backgroundLine->boundingRect().width()*(m_currentVal/m_maximum),
                                    m_backgroundLine->boundingRect().height());
            break;
        }
        break;
    }

    if (m_progressLowColour != nullptr && m_currentVal <= m_maximum*(m_lowPercent/100))
    {
        m_progressLine->setBrush(m_progressLowColour);
    }
    else
    {
        m_progressLine->setBrush(m_progressColour);
    }
}

void ProgressBar::setMaximum(qreal maximum)
{
    m_maximum = maximum;
}

void ProgressBar::setValue(qreal value)
{
    if (m_newVal != value)
    {
        m_newVal = value;
        if (value > m_maximum)
        {
            m_newVal = m_maximum;
        }
        else if (value < 0)
        {
            m_newVal = 0;
        }
        m_changeInVal = (m_newVal - m_currentVal)/m_progressChangeRate;
    }
}

void ProgressBar::setProgressColour(QColor colour)
{
    if (m_progressColour == m_progressLowColour)
    {
        m_progressLowColour = colour;
    }
    m_progressColour = colour;
}

void ProgressBar::setProgressLowColour(QColor colour)
{
    m_progressLowColour = colour;
}

void ProgressBar::setBackgroundColour(QColor colour)
{
    m_backgroundLine->setBrush(colour);
}

void ProgressBar::setOutlineColour(QColor colour)
{
    setBrush(colour);
}

void ProgressBar::setLowPercent(qreal percent)
{
    m_lowPercent = percent;
}

void ProgressBar::setDirection(Direction dir)
{
    m_direction = dir;
}

void ProgressBar::setAllignment(Allignment allignment)
{
    m_allignment = allignment;
}

void ProgressBar::setRect(qreal x, qreal y, qreal w, qreal h)
{
    QGraphicsRectItem::setRect(x,y,w,h);
    switch (m_allignment)
    {
    case Vertical:
        m_backgroundLine->setRect(-5*GLOBAL::Scale,0, w-2*GLOBAL::Scale, h-2*GLOBAL::Scale);
        break;
    case Horizontal:
        m_backgroundLine->setRect(0,-5*GLOBAL::Scale, w-2*GLOBAL::Scale, h-2*GLOBAL::Scale);
        break;
    }
}

void ProgressBar::setRect(const QRectF &rect)
{
    QGraphicsRectItem::setRect(rect);
    switch (m_allignment)
    {
    case Vertical:
        m_backgroundLine->setRect(-5*GLOBAL::Scale,0, rect.width()-2*GLOBAL::Scale, rect.height()-2*GLOBAL::Scale);
        break;
    case Horizontal:
        m_backgroundLine->setRect(0,-5*GLOBAL::Scale, rect.width()-2*GLOBAL::Scale, rect.height()-2*GLOBAL::Scale);
        break;
    }}

int ProgressBar::getValue()
{
    return m_currentVal;
}

int ProgressBar::getMaximum()
{
    return m_maximum;
}
