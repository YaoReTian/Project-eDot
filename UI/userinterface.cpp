#include "userinterface.h"

#include "../Utils/global.h"

UserInterface::UserInterface() : m_popupPosX(0), m_popupPosY(0), m_elapsedTime(0)
{
    m_popupMenu = new ButtonMenu;
}

UserInterface::~UserInterface()
{
    delete m_popupMenu;
}

void UserInterface::removeItem(QGraphicsScene &scene)
{
    m_popupMenu->removeItem(scene);
}

void UserInterface::input(KeyMap* keys, QGraphicsItem* activeCharacter)
{
    m_popupMenu->input(keys);
    m_popupPosX = activeCharacter->x() + activeCharacter->boundingRect().width() + 5* GLOBAL::Scale;
    m_popupPosY = activeCharacter->y();
}

void UserInterface::update(int deltatime)
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
        m_popupMenu->setPos(m_popupPosX, m_popupPosY);
        m_popupMenu->update(deltatime);
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
