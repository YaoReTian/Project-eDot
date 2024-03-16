#include "sprite.h"
#include "global.h"
#include "tile.h"

Sprite::Sprite(QGraphicsItem * parent) : QObject(), QGraphicsPixmapItem(parent)
{
    m_elapsed_time = 0;
    m_currentStateName = "idle";
    m_currentFrame = 0;
    m_interactable = false;
    m_type = "Sprite";
    setZValue(GLOBAL::TILE_LAYER);
}

void Sprite::setID(int ID)
{
    m_SpriteID = ID;
}

void Sprite::setName(QString name)
{
    m_name = name;
}

void Sprite::setType(QString type)
{
    m_type = type;
}

int Sprite::getID()
{
    return m_SpriteID;
}

QString Sprite::getName()
{
    return m_name;
}

QString Sprite::getType()
{
    return m_type;
}
void Sprite::setSpriteSheet(QPixmap spriteSheet)
{
    m_spriteSheet = spriteSheet;
}

void Sprite::setFrameSize(QSize frameSize)
{
    m_frameSize = frameSize;
}

void Sprite::setInteraction(QString text, QString dialogue)
{
    m_interactable = true;
    m_interactText = text;
    m_interactDialogue = dialogue;
}

void Sprite::addAnimationState(QString stateName, int startFrame, int endFrame, float frameTime)
{
    m_states[stateName] = new AnimationState;
    m_states[stateName]->stateName = stateName;
    m_states[stateName]->frameTime = frameTime;
    int y = 0;
    int x = 0;

    endFrame++;

    // initialise coordinates
    for (int n = 0; n < startFrame; n++)
    {
        x++;
        if (x*m_frameSize.width() >= m_spriteSheet.width())
        {
            x = 0;
            y ++;
        }
    }

    // Adds pixmaps
    for (int n = startFrame; n < endFrame; n++)
    {
        m_states[stateName]->frames.append(m_spriteSheet.copy(x*m_frameSize.width(),y*m_frameSize.height(),m_frameSize.width(),m_frameSize.height())
                                               .scaled(m_frameSize.width()*GLOBAL::Scale, m_frameSize.height()*GLOBAL::Scale));
        x++;
        if (x*m_frameSize.width() >= m_spriteSheet.width())
        {
            x = 0;
            y ++;
        }
    }
}

void Sprite::addTransition(QString startStateName, GLOBAL::Action action, QString endStateName)
{
    m_states[startStateName]->transitions[action] = endStateName;
}

void Sprite::update(int deltaTime)
{
    m_elapsed_time += deltaTime;

    if (m_states[m_currentStateName]->frameTime != -1 &&
        m_states[m_currentStateName]->frameTime <= m_elapsed_time)
    {
        m_elapsed_time = 0;
        m_currentFrame++;
        if (m_currentFrame  == m_states[m_currentStateName]->frames.size())
        {
            m_currentFrame = 0;
        }
        setPixmap(m_states[m_currentStateName]->frames[m_currentFrame]);
    }
    else if (m_states[m_currentStateName]->frameTime == -1)
    {
        setPixmap(m_states[m_currentStateName]->frames[m_currentFrame]);
    }
}

void Sprite::setAction(GLOBAL::Action action)
{
    if (m_states[m_currentStateName]->transitions.contains(action))
    {
        m_currentStateName = m_states[m_currentStateName]->transitions[action];
    }
}
