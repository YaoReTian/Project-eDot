#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include "button.h"
#include "../gameobject.h"
#include <QQueue>
#include <QObject>

class ButtonManager : public QObject, public GameObject
{
    Q_OBJECT
public:
    ButtonManager(QGraphicsItem *parent = 0);
    ~ButtonManager();
    void setBaseZ(qreal z);
    void setPos(QPointF pos);
    void setPos(qreal x, qreal y);
    qreal width();
    qreal height();

    Button* getButtonFromPool();
    void addButton(Button* b);
    void clear();

    void input(KeyMap* keys);
    virtual void update(int deltatime) override;

    void setParentItem(QGraphicsItem* parent);
    int numActiveButtons();

public slots:
    void hide();
    void show();

private:
    QList<Button*> m_activeButtons;
    QQueue<Button*> m_pool;
    QGraphicsItem* m_parent;
    qreal m_baseZ;
    int m_focusedIndex;
    int m_elapsedTime;
    QPointF m_pos;
    bool m_hidden;
};

#endif // BUTTONMANAGER_H
