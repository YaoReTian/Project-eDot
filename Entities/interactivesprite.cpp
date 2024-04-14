#include "interactivesprite.h"

InteractiveSprite::InteractiveSprite()
    : Sprite(), m_interactive(false), m_button(nullptr), m_interactingWithPlayer(false)
{
}

InteractiveSprite::~InteractiveSprite()
{
    delete m_button;
}

void InteractiveSprite::update(int deltatime)
{
    Sprite::update(deltatime);
    if (m_interactive && m_button->isActive())
    {
        if (m_button->isTriggered())    m_interactingWithPlayer = true;
        m_button->setActive(false);
    }
}

void InteractiveSprite::setAction(GLOBAL::Action action)
{
    if (m_interactive && m_button->isTriggered()) action = GLOBAL::NONE;
    Sprite::setAction(action);
}

void InteractiveSprite::setInteraction(QString text, QString script)
{
    m_interactive = true;
    m_interactText = (text == "") ? getName() : text;
    m_script = script;

    m_button = new Button;
    m_button->setText(m_interactText);
}

QString InteractiveSprite::getScript()
{
    return m_script;
}

bool InteractiveSprite::isInteractive()
{
    return m_interactive;
}

bool InteractiveSprite::isInteractingWithPlayer()
{
    return m_interactingWithPlayer;
}

Button* InteractiveSprite::getButton()
{
    return m_button;
}

void InteractiveSprite::popup()
{
    m_button->setActive(true);
}
