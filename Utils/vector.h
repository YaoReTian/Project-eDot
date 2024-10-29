#ifndef VECTOR_H
#define VECTOR_H

#include <QtGlobal>

class Vector
{
public:
    Vector(qreal i=0, qreal j=0);

    void setVector(qreal i, qreal j);
    void setVector(Vector v);
    void setI(qreal i);
    void setJ(qreal j);
    void toUnitVector();

    qreal magnitude() const;
    qreal i() const;
    qreal j() const;
    qreal angle() const;
    bool null() const;

    Vector operator+(const Vector &other) const;
    Vector &operator+=(const Vector &other);
    Vector operator-(const Vector &other) const;
    Vector operator-() const;
    Vector &operator-=(const Vector &other);
    Vector operator*(const qreal &val) const;
    Vector &operator*=(const qreal &val);
    Vector operator/(const qreal &val) const;
    Vector &operator/=(const qreal &val);

    bool operator==(const Vector &other) const;
    bool operator!=(const Vector &other) const;

private:
    qreal m_i;
    qreal m_j;
};

#endif // VECTOR_H
