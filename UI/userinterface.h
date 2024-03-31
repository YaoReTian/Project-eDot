#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "button.h"
#include "buttonmenu.h"

struct PopupInteraction
{
    Button* m_button;
    QString m_interactDialogue;
    bool m_rendered = false;
};

class UserInterface
{
public:
    UserInterface(QGraphicsScene *scene);

    void update(int deltatime, KeyMap * keys, QGraphicsItem* activeCharacter);

    // Popup interactions
    void addPopupInteraction(QString spriteIdentifier, Button * button, QString interactDialogue);
    void renderPopupInteraction(QString spriteIdentifier);
    void removePopupInteractionFromScene(QString spriteIdentifier);
    bool popupRendered(QString spriteIdnetifier);
    bool popupTriggered(QString spriteIdentifier);

private:
    QMap<QString, PopupInteraction*> m_popupInteractions;
    QGraphicsScene * m_scene;
    int m_elapsedTime;
    ButtonMenu * m_popupMenu;
};

#endif // USERINTERFACE_H
