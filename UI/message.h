#ifndef MESSAGE_H
#define MESSAGE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include "../gameobject.h"
#include "../Utils/keymap.h"
#include <QQueue>

class Message : public QObject, public GameObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Message(QGraphicsItem* parent = 0);
    void setName(QString name);
    void setDialogue(QString dialogue);

    void input(KeyMap* keys);
    virtual void update(int deltatime) override;
    bool isComplete();

    enum FinishCondition
    {
        timeout,
        keyPress,
        signal
    };

    void setFinishCondition(FinishCondition condition);
    FinishCondition finishCondition();
    void nextDialogue();

signals:
    void complete();
    void hiding();

public slots:
    void reset();
    void hide();
    void receiveSignal();

private:
    QPixmap m_default;
    QPixmap m_namedTextbox;
    QQueue<QString> m_dialogue;
    QGraphicsTextItem m_nameItem;
    QGraphicsTextItem m_dialogueItem;
    QGraphicsTextItem m_continueMessage;
    int m_elapsedTime;
    int m_lengthOnScreen;
    bool m_dialogueComplete;
    FinishCondition m_finishCondition;
    int m_timeout;
};

#endif // MESSAGE_H
