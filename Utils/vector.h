#ifndef VECTOR_H
#define VECTOR_H

#include <QtGlobal>
#include <QPointF>

class Vector
{
public:
    Vector(qreal i=0, qreal j=0);
    Vector(QPointF p1, QPointF p2);

    void setVector(qreal i, qreal j);
    void setVector(Vector v);
    void setVector(QPointF p1, QPointF p2);
    void setI(qreal i);
    void setJ(qreal j);
    void toUnitVector();

    qreal magnitude() const;
    qreal i() const;
    qreal j() const;
    qreal angle() const;
    bool null() const;
    qreal dotProduct(Vector &other) const;
    qreal dotProduct(qreal i, qreal j) const;

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
