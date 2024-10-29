#include "vector.h"

#include <cmath>
#include <QtMath>
#include <QDebug>

Vector::Vector(qreal i, qreal j)
    : m_i(i), m_j(j)
{

}

void Vector::setVector(qreal i, qreal j)
{
    m_i = i;
    m_j = j;
}

void Vector::setVector(Vector v)
{
    m_i = v.i();
    m_j = v.j();
}

void Vector::setI(qreal i)
{
    m_i = i;
}

void Vector::setJ(qreal j)
{
    m_j = j;
}

void Vector::toUnitVector()
{
    qreal mag = magnitude();
    if (mag != 0)
    {
        m_i /= mag;
        m_j /= mag;
    }
}

qreal Vector::magnitude() const
{
    return sqrt(pow(m_i,2) + pow(m_j, 2));
}

qreal Vector::i() const
{
    return m_i;
}

qreal Vector::j() const
{
    return m_j;
}

qreal Vector::angle() const
{
    if (m_j == 0 && m_i == 0)
    {
        return 0;
    }
    else if (m_j == 0)
    {
        return (m_i > 0) ? 90 : -90;
    }
    else if (m_i == 0)
    {
        return (m_j > 0) ? 180 : 0;
    }

    qreal theta = qRadiansToDegrees(qAtan(qAbs(m_j/m_i)));
    if (m_i > 0)
    {
        return (m_j > 0) ? 90 + theta : 90 - theta;
    }

    return (m_j > 0) ? -90 - theta : theta - 90;
}

bool Vector::null() const
{
    return bool(m_i ==0 && m_j==0);
}

Vector Vector::operator+(const Vector &other) const
{
    Vector v(m_i + other.i(), m_j+other.j());
    return v;
}

Vector &Vector::operator+=(const Vector &other)
{
    m_i += other.i();
    m_j += other.j();
    return *this;
}

Vector Vector::operator-(const Vector &other) const
{
    Vector v(m_i - other.i(), m_j - other.j());
    return v;
}

Vector Vector::operator -() const
{
    Vector v(-m_i, -m_j);
    return v;
}

Vector &Vector::operator-=(const Vector &other)
{
    m_i -= other.i();
    m_j -= other.j();
    return *this;
}

Vector Vector::operator*(const qreal &val) const
{
    Vector v(m_i*val, m_j*val);
    return v;
}

Vector &Vector::operator *=(const qreal &val)
{
    m_i *= val;
    m_j *= val;
    return *this;
}

Vector Vector::operator /(const qreal &val) const
{
    return Vector(m_i/val, m_j/val);
}

Vector &Vector::operator /=(const qreal &val)
{
    m_i /= val;
    m_j /= val;
    return *this;
}

bool Vector::operator==(const Vector &other) const
{
    if (m_i == other.i() && m_j == other.j())    return true;
    else    return false;
}

bool Vector::operator!=(const Vector &other) const
{
    if (m_i != other.i() || m_j != other.j())   return true;
    else    return false;
}
