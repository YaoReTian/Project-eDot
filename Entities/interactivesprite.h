#ifndef INTERACTIVESPRITE_H
#define INTERACTIVESPRITE_H

#include "sprite.h"

class InteractiveSprite : public Sprite
{
public:
    InteractiveSprite();

    // Overridden functions
    void update(int deltatime);
    void setAction(GLOBAL::Action action);

    // For interactions with the player
    QString getScript();
    void setInteraction(QString text, QString script);
    Button* getButton();
    bool isInteractive();
    bool isInteractingWithPlayer();
    void popup();

private:
    bool m_interactable;
    QString m_interactText;
    QString m_script;

    // For interactions
    Button * m_button;
    bool m_interactingWithPlayer;
};

#endif // INTERACTIVESPRITE_H
