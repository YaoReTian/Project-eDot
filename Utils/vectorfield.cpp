#include "vectorfield.h"
#include <QQueue>
#include <QDebug>
#include <QtMath>

VectorField::VectorField()
    : m_parentF({nullptr, nullptr}), m_fieldEq({"",""})
{

}

VectorField::VectorField(QString f_i, QString f_j)
{
    VectorField();
    setField(f_i, f_j);
}

void VectorField::setField(QString f_i, QString f_j)
{
    m_fieldEq[0] = f_i;
    m_fieldEq[1] = f_j;
    m_parentF[0] = toRPNtree(f_i);
    m_parentF[1] = toRPNtree(f_j);
}

Vector* VectorField::getVector(qreal a_x, qreal a_y, qreal p_x, qreal p_y)
{
    Vector* v = new Vector;
    m_var["x"] = a_x;
    m_var["y"] = a_y;
    m_var["px"] = p_x;
    m_var["py"] = p_y;
    findVectorValue(m_parentF[0]);
    v->m_i = m_RPNstack.pop();
    findVectorValue(m_parentF[1]);
    v->m_j = m_RPNstack.pop();
    return v;
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
    for (const QChar c : str)
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
        else if (operations.keys().indexOf(c) != -1 || c == '(' || c==')')
        {
            if (tempStr.size() > 0)
            {
                if (!(functions.contains(tempStr) ||
                      m_var.keys().contains(tempStr) ||
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
        else if (parsed.head().back().isDigit() || m_var.keys().contains(parsed.head()))
        {
            nodeStack.push(new Node(parsed.head()));
        }
        else if (parsed.head()== "-" && prev == "(")
        {
            nodeStack.push(new Node("0"));
            symStack.push(parsed.head());
        }
        else if (operations.keys().indexOf(parsed.head()) != -1)
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
            if (!symStack.empty() && functions.indexOf(symStack.top()) != -1)
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

    if (operations.keys().indexOf(parent->m_data) != -1)
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
