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
    bool m_active;
};

class UserInterface
{
public:
    UserInterface();

    void removeItem(QGraphicsScene &scene);
    void update(int deltatime, KeyMap * keys, QGraphicsItem* activeCharacter);
    void render(QGraphicsScene &scene);

    // Popup interactions
    void addPopup(QString spriteIdentifier, Button * button, QString interactDialogue);
    void renderPopup(QString spriteIdentifier);
    void hidePopup(QString spriteIdentifier);
    void removePopup(QString spriteIdentifier);

    bool popupActive(QString spriteIdnetifier);
    bool popupTriggered(QString spriteIdentifier);

private:
    QMap<QString, Popup*> m_popups;
    int m_elapsedTime;
    ButtonMenu * m_popupMenu;
};

#endif // USERINTERFACE_H
