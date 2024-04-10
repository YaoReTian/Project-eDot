#include "sprite.h"

Sprite::Sprite(QGraphicsItem * parent) : QObject(), QGraphicsPixmapItem(parent)
{
    m_elapsed_time = 0;
    m_currentStateName = "idle";
    m_currentFrame = 0;
    m_interactable = false;
    m_type = "Sprite";
    setZValue(GLOBAL::SPRITE_LAYER + y());
    m_interactingWithPlayer = false;
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

Button* Sprite::getButton()
{
    return m_button;
}

QString Sprite::getScript()
{
    return m_script;
}

void Sprite::setSpriteSheet(QPixmap spriteSheet)
{
    m_spriteSheet = spriteSheet;
}

void Sprite::setFrameSize(QSize frameSize)
{
    m_frameSize = frameSize;
}

void Sprite::setInteraction(QString text, QString script)
{
    m_interactable = true;
    m_interactText = (text == "") ? m_name : text;
    m_script = script;

    m_button = new Button;
    m_button->setText(m_interactText);
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

void Sprite::removeItem(QGraphicsScene &scene)
{
    scene.removeItem(this);
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

    // Check for interactions
    if (m_interactable && m_button->isActive())
    {
        if (m_button->isTriggered())    m_interactingWithPlayer = true;
        m_button->setActive(false);
    }
}

void Sprite::render(QGraphicsScene &scene)
{
    scene.addItem(this);
}

void Sprite::setAction(GLOBAL::Action action)
{
    if (m_interactable && m_button->isTriggered()) action = GLOBAL::NONE;
    if (m_states[m_currentStateName]->transitions.contains(action))
    {
        m_currentStateName = m_states[m_currentStateName]->transitions[action];
        m_currentFrame = 0;
    }
}

bool Sprite::isInteractable()
{
    return m_interactable;
}

bool Sprite::isInteractingWithPlayer()
{
    return m_interactingWithPlayer;
}

void Sprite::popup()
{
    m_button->setActive(true);
}
