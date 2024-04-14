#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "button.h"
#include "buttonmenu.h"
#include "../gameobject.h"

struct Popup
{
    Button* m_button;
    QString m_script;
};

class UserInterface : public GameObject
{
public:
    UserInterface();
    ~UserInterface();

    void input(KeyMap* keys, QGraphicsItem* activeCharacter);

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltatime) override;
    virtual void render(QGraphicsScene &scene) override;

    // Popup interactions
    void addPopup(Button * button, QString interactDialogue);

private:
    qreal m_popupPosX;
    qreal m_popupPosY;
    QList<Popup*> m_popups;
    int m_elapsedTime;
    ButtonMenu* m_popupMenu;
};

#endif // USERINTERFACE_H
