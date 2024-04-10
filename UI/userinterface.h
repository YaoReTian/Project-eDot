#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "button.h"
#include "buttonmenu.h"

struct Popup
{
    Button* m_button;
    QString m_script;
};

class UserInterface
{
public:
    UserInterface();

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime, KeyMap * keys, QGraphicsItem* activeCharacter);
    void render(QGraphicsScene &scene);

    // Popup interactions
    void addPopup(Button * button, QString interactDialogue);

private:
    QList<Popup*> m_popups;
    int m_elapsedTime;
    ButtonMenu * m_popupMenu;
};

#endif // USERINTERFACE_H
