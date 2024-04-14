#ifndef INTERACTIVESPRITE_H
#define INTERACTIVESPRITE_H

#include "sprite.h"

#include "../UI/button.h"

class InteractiveSprite : public Sprite
{
public:
    InteractiveSprite();
    ~InteractiveSprite();

    // Overridden functions
    virtual void update(int deltatime) override;
    void setAction(GLOBAL::Action action);

    // For interactions with the player
    QString getScript();
    void setInteraction(QString text, QString script);
    Button* getButton();
    bool isInteractive();
    bool isInteractingWithPlayer();
    void popup();

private:
    bool m_interactive;
    QString m_interactText;
    QString m_script;

    // For interactions
    Button * m_button;
    bool m_interactingWithPlayer;
};

#endif // INTERACTIVESPRITE_H
