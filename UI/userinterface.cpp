#include "userinterface.h"

#include "../Utils/global.h"

UserInterface::UserInterface()
{
    m_popupMenu = new ButtonMenu;
}

void UserInterface::removeItem(QGraphicsScene &scene)
{
    if (m_popupMenu->isActive())    m_popupMenu->removeItem(scene);
}

void UserInterface::update(int deltatime, KeyMap * keys, QGraphicsItem * activeCharacter)
{
    if (m_popupMenu->isActive())
    {
        m_popupMenu->setPos(activeCharacter->x() + activeCharacter->boundingRect().width() + 5* GLOBAL::Scale,
                            activeCharacter->y());
        m_popupMenu->update(deltatime, keys);
        if (m_popupMenu->buttonReleased())
        {
            m_popupMenu->setActive(false);
            for ( auto [key, value] : m_popups.asKeyValueRange())
            {
                value->m_button->pause();
                value->m_active = false;
            }
        }
    }
}

void UserInterface::render(QGraphicsScene &scene)
{
    if (m_popupMenu->isActive())    m_popupMenu->render(scene);
}

void UserInterface::addPopup(QString spriteIdentifier, Button * button, QString interactDialogue)
{
    m_popups[spriteIdentifier] = new Popup;
    m_popups[spriteIdentifier]->m_button = button;
    m_popups[spriteIdentifier]->m_button->setIconText("F");
    m_popups[spriteIdentifier]->m_script = interactDialogue;
}

void UserInterface::renderPopup(QString spriteIdentifier)
{
    if (m_popups.contains(spriteIdentifier))
    {
        m_popups[spriteIdentifier]->m_active = true;
        m_popupMenu->addButton(m_popups[spriteIdentifier]->m_button);
        if (!m_popupMenu->isActive())
        {
            m_popupMenu->setActive(true);
        }
    }
    else
    {
        qDebug() << "Error: sprite popup button not loaded to UI";
    }
}

void UserInterface::hidePopup(QString spriteIdentifier)
{
    if (m_popups.contains(spriteIdentifier) &&
        m_popupMenu->contains(m_popups[spriteIdentifier]->m_button))
    {
        m_popups[spriteIdentifier]->m_active = false;
        m_popupMenu->removeButton(m_popups[spriteIdentifier]->m_button);

        if (m_popupMenu->numberOfButtons() == 0)
        {
            m_popupMenu->setActive(false);
        }
    }
}

void UserInterface::removePopup(QString spriteIdentifier)
{
    if (m_popups.contains(spriteIdentifier))
    {
        hidePopup(spriteIdentifier);
        m_popups.remove(spriteIdentifier);
    }
}

bool UserInterface::popupActive(QString spriteIdentifier)
{
    return m_popups[spriteIdentifier]->m_active;
}

bool UserInterface::popupTriggered(QString spriteIdentifier)
{
    return m_popups[spriteIdentifier]->m_button->isTriggered();
}
