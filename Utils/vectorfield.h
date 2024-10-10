#ifndef VECTORFIELD_H
#define VECTORFIELD_H
#define _USE_MATH_DEFINES

#include <cmath>
#include <QStack>
#include <QList>
#include <QMap>
#include <QPointF>

#include "vector.h"
#include <QDebug>

struct Node
{
    QString m_data;
    Node* m_left = nullptr;
    Node* m_right = nullptr;

    Node(QString data = "") : m_data(data) {}
    ~Node()
    {
        qDebug() << "Test";
        delete m_left;
        delete m_right;
    }
};

class VectorField
{
public:
    VectorField();
    VectorField(QString f_i, QString f_j);
    ~VectorField();

    void setField(QString f_i, QString f_j);
    void setOrigin(qreal x, qreal y);
    void setOrigin(QPointF point);
    Vector getVector(qreal a_x = 0, qreal a_y = 0, qreal p_x = 0, qreal p_y = 0);

private:
    void clearNodes(Node* &parent);
    const QList<QString> functions = {"sin", "cos", "tan", "ln", "log10",
                                      "acos", "asin", "atan", "sqrt"};
    const QMap<QString, int> operations = {
        {"+",1},
        {"-",1},
        {"/",2},
        {"*",2},
        {"^",3}
    };

    Node* toRPNtree(QString str);
    void findVectorValue(Node* parent);
    Node* m_parentF[2];
    QString m_fieldEq[2];
    QPointF m_origin;

    // temporary calculation storage
    QStack<qreal> m_RPNstack;
    QMap<QString, qreal> m_var = {
        {"x", 0},
        {"y", 0},
        {"px",0},
        {"py",0},
        {"pi", M_PI},
        {"e", M_E}
    };
};

#endif // VECTORFIELD_H
