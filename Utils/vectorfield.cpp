#include "vectorfield.h"
#include <QQueue>
#include <QDebug>
#include <QtMath>

VectorField::VectorField()
    : m_origin(0,0)
{
    m_parentF[0] = nullptr;
    m_parentF[1] = nullptr;
    m_fieldEq[0] = "";
    m_fieldEq[1] = "";
}

VectorField::VectorField(QString f_i, QString f_j)
{
    VectorField();
    setField(f_i, f_j);
}

VectorField::~VectorField()
{
    clearNodes(m_parentF[0]);
    clearNodes(m_parentF[1]);
}

void VectorField::setField(QString f_i, QString f_j)
{
    m_fieldEq[0] = f_i;
    m_fieldEq[1] = f_j;
    m_parentF[0] = toRPNtree(f_i);
    m_parentF[1] = toRPNtree(f_j);
}

void VectorField::setOrigin(QPointF point)
{
    m_origin = point;
}

void VectorField::setOrigin(qreal x, qreal y)
{
    m_origin.setX(x);
    m_origin.setY(y);
}

Vector VectorField::getVector(qreal a_x, qreal a_y)
{
    Vector v;
    m_var["x"] = a_x - m_origin.x();
    m_var["y"] = a_y - m_origin.y();
    findVectorValue(m_parentF[0]);
    v.setI(m_RPNstack.pop());
    findVectorValue(m_parentF[1]);
    v.setJ(m_RPNstack.pop());
    return v;
}

Vector VectorField::getVector(QPointF pos)
{
    return getVector(pos.x(), pos.y());
}

Node* VectorField::toRPNtree(QString str)
{
    QQueue<QString> parsed;
    QStack<QString> symStack;
    QStack<Node*> nodeStack;
    QString tempStr;

    str = str.replace(" ","");
    str = "(" + str + ")";

    // Parse into queue form
    for (const QChar c : std::as_const(str))
    {
        if (c.isLetter())
        {
            if (tempStr.size()>0 && !tempStr.back().isLetter() && !(tempStr.back() == '-'))
            {
                throw std::invalid_argument("Invalid function format");
            }
            tempStr.append(c);
        }
        else if (c.isDigit())
        {
            if (tempStr.size()>0 && !tempStr.back().isDigit() && !(tempStr.back() == '-'))
            {
                throw std::invalid_argument("Invalid function format");
            }
            tempStr.append(c);
        }
        else if (operations.contains(c) || c == '(' || c==')')
        {
            if (tempStr.size() > 0)
            {
                if (!(functions.contains(tempStr) ||
                      m_var.contains(tempStr) ||
                      tempStr.back().isDigit() ))
                {
                    qDebug() << tempStr;
                    throw std::invalid_argument("Invalid function format");
                }
                parsed.enqueue(tempStr);
            }
            parsed.enqueue(c);
            tempStr = "";
        }
    }
    if (tempStr.size() > 0)
    {
        parsed.enqueue(tempStr);
    }

    // Convert to expression tree
    Node* tempNode;
    QString prev;

    while (!parsed.empty())
    {
        if (parsed.head() == "(" || functions.indexOf(parsed.head()) != -1)
        {
            symStack.push(parsed.head());
        }
        else if (parsed.head().back().isDigit() || m_var.contains(parsed.head()))
        {
            nodeStack.push(new Node(parsed.head()));
        }
        else if (parsed.head()== "-" && prev == "(")
        {
            nodeStack.push(new Node("0"));
            symStack.push(parsed.head());
        }
        else if (operations.contains(parsed.head()))
        {
            while (!symStack.empty() && symStack.top() != "(" &&
                   ((parsed.head() != "^" && operations[symStack.top()] >= operations[parsed.head()]) ||
                    (parsed.head() == "^" && operations[symStack.top()] > operations[parsed.head()])))
            {
                tempNode = new Node(symStack.pop());
                tempNode->m_right = nodeStack.pop();
                tempNode->m_left = nodeStack.pop();
                nodeStack.push(tempNode);
            }
            symStack.push(parsed.head());
        }
        else if (parsed.head() == ")")
        {
            while (!symStack.empty() && symStack.top() != "(")
            {
                tempNode = new Node(symStack.pop());
                tempNode->m_right = nodeStack.pop();
                tempNode->m_left = nodeStack.pop();
                nodeStack.push(tempNode);
            }
            if (symStack.empty())   throw std::invalid_argument("Invalid brackets");
            symStack.pop();
            if (!symStack.empty() && functions.contains(symStack.top()))
            {
                tempNode = new Node(symStack.pop());
                tempNode->m_right = nodeStack.pop();
                nodeStack.push(tempNode);
            }
        }
        prev = parsed.dequeue();
    }

    return nodeStack.pop();
}

void VectorField::findVectorValue(Node *parent)
{
    if (parent == nullptr) return;
    findVectorValue(parent->m_left);
    findVectorValue(parent->m_right);

    if (operations.contains(parent->m_data))
    {
        qreal tempVal = 0;
        if (parent->m_data == "+")
        {
            tempVal = m_RPNstack.pop() + m_RPNstack.pop();
        }
        else if (parent->m_data == "-")
        {
            tempVal = m_RPNstack.pop();
            tempVal = m_RPNstack.pop() - tempVal;
        }
        else if (parent->m_data == "/")
        {
            tempVal = m_RPNstack.pop();
            tempVal = m_RPNstack.pop() / tempVal;
        }
        else if (parent->m_data == "*")
        {
            tempVal = m_RPNstack.pop() * m_RPNstack.pop();
        }
        else // ^
        {
            tempVal = m_RPNstack.pop();
            tempVal = pow(m_RPNstack.pop(), tempVal);
        }
        m_RPNstack.push(tempVal);
    }
    else if (functions.indexOf(parent->m_data) != -1)
    {
        if (parent->m_data == "sin")
        {
            m_RPNstack.push(qSin(m_RPNstack.pop()));
        }
        else if (parent->m_data == "cos")
        {
            m_RPNstack.push(qCos(m_RPNstack.pop()));
        }
        else if (parent->m_data == "tan")
        {
            m_RPNstack.push(qTan(m_RPNstack.pop()));
        }
        else if (parent->m_data == "ln")
        {
            m_RPNstack.push(qLn(m_RPNstack.pop()));
        }
        else if (parent->m_data == "log10")
        {
            m_RPNstack.push(std::log10(m_RPNstack.pop()));
        }
        else if (parent->m_data == "acos")
        {
            m_RPNstack.push(qAcos(m_RPNstack.pop()));
        }
        else if (parent->m_data == "asin")
        {
            m_RPNstack.push(qAsin(m_RPNstack.pop()));
        }
        else if (parent->m_data == "atan")
        {
            m_RPNstack.push(qAtan(m_RPNstack.pop()));
        }
        else if (parent->m_data == "sqrt")
        {
            m_RPNstack.push(sqrt(m_RPNstack.pop()));
        }
    }
    else if (parent->m_data.back().isDigit())
    {
        m_RPNstack.push(parent->m_data.toDouble());
    }
    else
    {
        m_RPNstack.push(m_var[parent->m_data]);
    }
}


void VectorField::clearNodes(Node* &parent)
{
    if (parent == nullptr)  return;
    clearNodes(parent->m_left);
    clearNodes(parent->m_right);

    delete parent;
    parent = nullptr;
}

QPointF VectorField::origin()
{
    return m_origin;
}

QString VectorField::getPrefixI()
{
    return m_fieldEq[0];
}

QString VectorField::getPrefixJ()
{
    return m_fieldEq[1];
}

QString VectorField::getPostfixI()
{
    return getPostfix(m_parentF[0]);
}

QString VectorField::getPostfixJ()
{
    return getPostfix(m_parentF[1]);
}

QString VectorField::getPostfix(Node *parent)
{
    if (parent == nullptr)  return "";
    QString s = "";
    s+= getPostfix(parent->m_left);
    s+= " ";
    s+= getPostfix(parent->m_right);

    s+= " ";
    s+= parent->m_data;
    return s;
}
