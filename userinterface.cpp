#include "userinterface.h"

#include "global.h"

UserInterface::UserInterface(QGraphicsScene * scene)
{
    m_scene = scene;
}

void UserInterface::update(int deltatime, KeyMap * keys, QGraphicsItem * activeCharacter)
{
    for (auto [key, value] : m_popupInteractions.asKeyValueRange())
    {
        if (value->m_rendered)
        {
            value->m_button->setPos(activeCharacter->x() + activeCharacter->boundingRect().width() + 5* GLOBAL::Scale,
                                    activeCharacter->y());
            value->m_button->update(deltatime, keys);
        }
    }
}

void UserInterface::addPopupInteraction(int SpriteID, Button * button, QString interactDialogue)
{
    m_popupInteractions[SpriteID] = new PopupInteraction;
    m_popupInteractions[SpriteID]->m_button = button;
    m_popupInteractions[SpriteID]->m_interactDialogue = interactDialogue;
}

void UserInterface::renderPopupInteraction(int SpriteID)
{
    m_popupInteractions[SpriteID]->m_button->render(*m_scene);
    m_popupInteractions[SpriteID]->m_button->setFocused();
    m_popupInteractions[SpriteID]->m_rendered = true;
}

void UserInterface::removePopupInteractionFromScene(int SpriteID)
{
    m_popupInteractions[SpriteID]->m_button->removeFromScene(*m_scene);
    m_popupInteractions[SpriteID]->m_rendered = false;
}

bool UserInterface::popupRendered(int SpriteID)
{
    return m_popupInteractions[SpriteID]->m_rendered;
}

bool UserInterface::popupClicked(int SpriteID)
{
    return m_popupInteractions[SpriteID]->m_button->isClicked();
}
