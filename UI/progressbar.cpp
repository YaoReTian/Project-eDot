#include "progressbar.h"

#include <QtMath>

#include "../Utils/global.h"

ProgressBar::ProgressBar()
    : m_elapsedTime(0), m_maximum(100), m_currentVal(0), m_changeInVal(0), m_newVal(0), m_lowPercent(20),
    m_x(0), m_y(0), m_zValue(0), m_width(0), m_length(0), m_progressLowColour(Qt::green), m_progressColour(Qt::green),
    m_allignment(Horizontal), m_direction(BottomToTop), m_progressChangeRate(1000.0f)
{
    m_progressLine = new QGraphicsRectItem;
    m_backgroundLine = new QGraphicsRectItem;
    m_outline = new QGraphicsRectItem;

    m_progressLine->setPen(Qt::NoPen);
    m_backgroundLine->setPen(Qt::NoPen);
    m_outline->setPen(QPen(Qt::white, 0.3f));
}

ProgressBar::~ProgressBar()
{
    delete m_progressLine;
    delete m_backgroundLine;
    delete m_outline;
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
        m_outline->setRect(m_x, m_y, m_width, m_length);
        m_backgroundLine->setRect(m_x+GLOBAL::Scale, m_y+GLOBAL::Scale, m_width-2*GLOBAL::Scale, m_length-2*GLOBAL::Scale);
        switch (m_direction)
        {
        case BottomToTop:
            m_progressLine->setRect(m_x+GLOBAL::Scale,
                                    m_y+m_length-GLOBAL::Scale-(m_length-2*GLOBAL::Scale)*(m_currentVal/m_maximum),
                                    m_width-2*GLOBAL::Scale,
                                    (m_length-2*GLOBAL::Scale)*(m_currentVal/m_maximum));
            break;
        case TopToBottom:
            m_progressLine->setRect(m_x+GLOBAL::Scale,
                                    m_y+GLOBAL::Scale,
                                    m_width-2*GLOBAL::Scale,
                                    (m_length-2*GLOBAL::Scale)*(m_currentVal/m_maximum));
            break;
        }
        break;
    case Horizontal:
        m_outline->setRect(m_x, m_y, m_length, m_width);
        m_backgroundLine->setRect(m_x+GLOBAL::Scale, m_y+GLOBAL::Scale, m_length-2*GLOBAL::Scale, m_width-2*GLOBAL::Scale);
        switch (m_direction)
        {
        case BottomToTop:
            m_progressLine->setRect(m_x+GLOBAL::Scale,
                                    m_y+GLOBAL::Scale,
                                    (m_length-2*GLOBAL::Scale)*(m_currentVal/m_maximum),
                                    m_width-2*GLOBAL::Scale);
            break;
        case TopToBottom:
            m_progressLine->setRect(m_x+m_length-GLOBAL::Scale-(m_length-2*GLOBAL::Scale)*(m_currentVal/m_maximum),
                                    m_y+GLOBAL::Scale,
                                    (m_length-2*GLOBAL::Scale)*(m_currentVal/m_maximum),
                                    m_width-2*GLOBAL::Scale);
            break;
        }
        break;
    }

    m_outline->setZValue(m_zValue);
    m_backgroundLine->setZValue(m_zValue+1);
    m_progressLine->setZValue(m_zValue+2);

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
    m_outline->setBrush(colour);
}

void ProgressBar::setLowPercent(qreal percent)
{
    m_lowPercent = percent;
}

void ProgressBar::setPos(qreal x, qreal y)
{
    m_x = x;
    m_y = y;
}

void ProgressBar::setZValue(qreal a_zValue)
{
    m_zValue = a_zValue;
}

void ProgressBar::setWidth(int width)
{
    m_width = width;
}

void ProgressBar::setLength(int length)
{
    m_length = length;
}

void ProgressBar::setDirection(Direction dir)
{
    m_direction = dir;
}

void ProgressBar::setAllignment(Allignment allignment)
{
    m_allignment = allignment;
}

int ProgressBar::getValue()
{
    return m_currentVal;
}

int ProgressBar::getMaximum()
{
    return m_maximum;
}

qreal ProgressBar::getWidth()
{
    return m_width;
}

qreal ProgressBar::getLength()
{
    return m_length;
}

qreal ProgressBar::x()
{
    return m_outline->x();
}

qreal ProgressBar::y()
{
    return m_outline->y();
}

qreal ProgressBar::zValue()
{
    return m_zValue;
}

QRectF ProgressBar::boundingRect()
{
    return m_outline->boundingRect();
}
