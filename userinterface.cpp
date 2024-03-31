#include "userinterface.h"

#include "global.h"

UserInterface::UserInterface(QGraphicsScene * scene)
{
    m_scene = scene;
    m_popupMenu = new ButtonMenu;
}

void UserInterface::update(int deltatime, KeyMap * keys, QGraphicsItem * activeCharacter)
{
    if (m_popupMenu->isRendered())
    {
        m_popupMenu->setPos(activeCharacter->x() + activeCharacter->boundingRect().width() + 5* GLOBAL::Scale,
                            activeCharacter->y());
        m_popupMenu->update(deltatime, keys, *m_scene);
        if (m_popupMenu->buttonReleased())
        {
            m_popupMenu->removeFromScene(*m_scene);
            for ( auto [key, value] : m_popupInteractions.asKeyValueRange())
            {
                value->m_button->pause();
                value->m_rendered = false;
            }
        }
    }
}

void UserInterface::addPopupInteraction(QString spriteIdentifier, Button * button, QString interactDialogue)
{
    m_popupInteractions[spriteIdentifier] = new PopupInteraction;
    m_popupInteractions[spriteIdentifier]->m_button = button;
    m_popupInteractions[spriteIdentifier]->m_button->setIconText("F");
    m_popupInteractions[spriteIdentifier]->m_interactDialogue = interactDialogue;
}

void UserInterface::renderPopupInteraction(QString spriteIdentifier)
{
    if (m_popupInteractions.contains(spriteIdentifier))
    {
        m_popupInteractions[spriteIdentifier]->m_rendered = true;
        m_popupMenu->addButton(m_popupInteractions[spriteIdentifier]->m_button);
        if (!m_popupMenu->isRendered())
        {
            m_popupMenu->render(*m_scene);
        }
    }
    else
    {
        qDebug() << "Error: sprite popup button not loaded to UI";
    }
}

void UserInterface::removePopupInteractionFromScene(QString spriteIdentifier)
{
    if (m_popupInteractions.contains(spriteIdentifier) &&
        m_popupMenu->contains(m_popupInteractions[spriteIdentifier]->m_button))
    {
        m_popupInteractions[spriteIdentifier]->m_rendered = false;
        m_popupMenu->removeButton(m_popupInteractions[spriteIdentifier]->m_button, *m_scene);
        if (m_popupMenu->numberOfButtons() == 0)
        {
            m_popupMenu->removeFromScene(*m_scene);
        }
    }
}

bool UserInterface::popupRendered(QString spriteIdentifier)
{
    return m_popupInteractions[spriteIdentifier]->m_rendered;
}

bool UserInterface::popupTriggered(QString spriteIdentifier)
{
    return m_popupInteractions[spriteIdentifier]->m_button->isTriggered();
}
