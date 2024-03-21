#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMap>
#include <QGraphicsScene>
#include <QGraphicsItem>

#include "button.h"
#include "keymap.h"

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
    void addPopupInteraction(int SpriteID, Button * button, QString interactDialogue);
    void renderPopupInteraction(int SpriteID);
    void removePopupInteractionFromScene(int SpriteID);
    bool popupRendered(int SpriteID);
    bool popupClicked(int SpriteID);

private:
    QMap<int, PopupInteraction*> m_popupInteractions;
    QGraphicsScene * m_scene;
};

#endif // USERINTERFACE_H
