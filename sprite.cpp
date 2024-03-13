#include "sprite.h"
#include "global.h"

Sprite::Sprite(QGraphicsItem * parent) : QObject(), QGraphicsPixmapItem(parent)
{
    m_elapsed_time = 0;
    m_currentStateIndex = 0;
    m_currentFrame = 0;
    m_interactable = false;
    m_type = "Sprite";
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
    m_states.append(new AnimationState);
    m_states.back()->stateName = stateName;
    m_states.back()->frameTime = frameTime;
    int y = 0;
    int x = 0;
    for (int n = startFrame; n < endFrame; n++)
    {
        m_states.back()->frames.append(m_spriteSheet.copy(x*m_frameSize.width(),y*m_frameSize.height(),m_frameSize.width(),m_frameSize.height())
                                .scaled(GLOBAL::ObjectSize));
        x++;
        if (x*m_frameSize.width() >= m_spriteSheet.width())
        {
            x = 0;
            y ++;
        }
    }
}

void Sprite::transition(QString stateName)
{
    int n = 0;
    bool found = false;
    while (n != m_states.size() && found == false)
    {
        if (m_states[n]->stateName == stateName) {
            m_currentStateIndex = n;
            m_currentFrame = 0;
            found = true;
        } else {
            n++;
        }
    }
}

void Sprite::update(int deltaTime)
{
    m_elapsed_time += deltaTime;

    QList<QGraphicsItem *> colliding_items = collidingItems();

    for (auto n : colliding_items) {
        //if (typeid(*n) == typeid(Enemy)) {
        //}
    }

    if (m_states[m_currentStateIndex]->frameTime <= m_elapsed_time)
    {
        m_elapsed_time = 0;
        m_currentFrame++;
        if (m_currentFrame  == m_states[m_currentStateIndex]->frames.size())
        {
            m_currentFrame = 0;
        }
        setPixmap(m_states[m_currentStateIndex]->frames[m_currentFrame]);
    }
}
