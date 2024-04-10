#include "userinterface.h"

#include "../Utils/global.h"

UserInterface::UserInterface()
{
    m_popupMenu = new ButtonMenu;
}

void UserInterface::removeItem(QGraphicsScene &scene)
{
    m_popupMenu->removeItem(scene);
}

void UserInterface::update(int deltatime, KeyMap * keys, QGraphicsItem * activeCharacter)
{
    for (const auto p : m_popups)
    {
        if (p->m_button->isActive() && !m_popupMenu->contains(p->m_button))
        {
            m_popupMenu->addButton(p->m_button);
        }
    }
    if (m_popupMenu->isActive())
    {
        m_popupMenu->setPos(activeCharacter->x() + activeCharacter->boundingRect().width() + 5* GLOBAL::Scale,
                            activeCharacter->y());
        m_popupMenu->update(deltatime, keys);
        if (m_popupMenu->buttonReleased())
        {
            m_popupMenu->setActive(false);
            for ( const auto p : m_popups)
            {
                p->m_button->pause();
            }
        }
    }
}

void UserInterface::render(QGraphicsScene &scene)
{
    m_popupMenu->render(scene);
}

void UserInterface::addPopup(Button * button, QString script)
{
    m_popups.append(new Popup);
    m_popups.back()->m_button = button;
    m_popups.back()->m_button->setIconText("F");
    m_popups.back()->m_script = script;
}
