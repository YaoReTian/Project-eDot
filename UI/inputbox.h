#ifndef INPUTBOX_H
#define INPUTBOX_H

#include <QGraphicsPixmapItem>
#include "../gameobject.h"
#include <QGraphicsTextItem>
#include "../Utils/keymap.h"

class InputBox : public QObject, public GameObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    InputBox(QGraphicsItem* parent = 0);

    void input(KeyMap* keys);
    virtual void update(int deltatime) override;

signals:
    void textInputted(QString txt);
    void invalidInput();

public slots:
    void show();
    void hide();

private:
    QString m_text;
    QGraphicsTextItem m_textBox;
    QGraphicsTextItem m_warning;

    int m_elapsedTime;
};

#endif // INPUTBOX_H
