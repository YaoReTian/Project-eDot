#include "message.h"

Message::Message(QGraphicsItem* parent)
    : QObject(), QGraphicsPixmapItem(parent),
    m_default(":/image/res/textbox.png"), m_namedTextbox(":/image/res/namedTextbox.png"),
    m_nameItem(new QGraphicsTextItem), m_dialogueItem(new QGraphicsTextItem),
    m_elapsedTime(0), m_lengthOnScreen(0), m_dialogueComplete(false),
    m_finishCondition(timeout), m_timeout(0)
{
    setPixmap(m_default);
    m_nameItem.setParentItem(this);
    m_dialogueItem.setParentItem(this);
    m_continueMessage.setParentItem(this);
    m_default = m_default.scaledToHeight(m_default.height()*GLOBAL::Scale);
    m_namedTextbox = m_namedTextbox.scaledToHeight(m_namedTextbox.height()*GLOBAL::Scale);

    m_nameItem.setPos(26*GLOBAL::Scale, 18*GLOBAL::Scale);
    m_dialogueItem.setPos(22*GLOBAL::Scale, 40*GLOBAL::Scale);
    m_dialogueItem.setTextWidth(418*GLOBAL::Scale);

    QFont font("rainyhearts", 9 * GLOBAL::Scale);
    font.setBold(true);
    m_nameItem.setDefaultTextColor(Qt::white);
    m_nameItem.setFont(font);
    m_dialogueItem.setDefaultTextColor(Qt::white);
    m_dialogueItem.setFont(font);

    font.setPointSize(5*GLOBAL::Scale);
    m_continueMessage.setDefaultTextColor(Qt::white);
    m_continueMessage.setFont(font);
    m_continueMessage.setPlainText("Press SPACE to continue.");
    m_continueMessage.setPos(230*GLOBAL::Scale - m_continueMessage.boundingRect().width()/2,
                             80*GLOBAL::Scale);
    m_continueMessage.hide();

    hide();
}

void Message::setName(QString name)
{
    m_nameItem.setPlainText(name);
    setPixmap(m_namedTextbox);
}

void Message::setDialogue(QString dialogue)
{
    m_dialogue.enqueue(dialogue);
}

void Message::input(KeyMap *keys)
{
    if (m_dialogue.empty() )
    {
        return;
    }

    if (m_dialogueComplete)
    {
        return;
    }

    if (keys->keyReleasedStatus(GLOBAL::SHOOT))
    {
        if (m_lengthOnScreen < m_dialogue.head().length())
        {
            m_lengthOnScreen = m_dialogue.head().length();
            m_dialogueItem.setPlainText(m_dialogue.head());
        }
        else if (m_dialogue.size() > 1 || m_finishCondition == keyPress)
        {
            nextDialogue();
        }
    }
}

void Message::update(int deltatime)
{
    if (m_dialogueComplete)
    {
        return;
    }

    m_elapsedTime += deltatime;

    if (m_dialogue.head().length() == m_lengthOnScreen)
    {
        m_timeout += deltatime;
        if (m_timeout >= 2000)
        {
            m_timeout = 0;
            if (!m_dialogue.empty() || m_finishCondition == timeout)
            {
                nextDialogue();
            }
            else if (m_finishCondition == keyPress)
            {
                m_continueMessage.show();
            }
        }
        else if (m_dialogue.size() == 1)
        {
            nextDialogue();
        }
    }

    if (m_elapsedTime >= 50 && m_lengthOnScreen < m_dialogue.head().length())
    {
        m_elapsedTime = 0;
        m_lengthOnScreen++;
        m_dialogueItem.setPlainText(m_dialogue.head().left(m_lengthOnScreen));
    }
}

bool Message::isComplete()
{
    return m_dialogueComplete;
}

void Message::setFinishCondition(FinishCondition condition)
{
    m_finishCondition = condition;
}

Message::FinishCondition Message::finishCondition()
{
    return m_finishCondition;
}

void Message::nextDialogue()
{
    m_continueMessage.hide();
    m_lengthOnScreen = 0;
    m_elapsedTime = 0;
    m_dialogue.dequeue();
    if (m_dialogue.empty())
    {
        emit complete();
        m_dialogueComplete = true;
        if (m_finishCondition != signal)    hide();
    }
}

// SLOTS

void Message::reset()
{
    m_continueMessage.hide();
    m_nameItem.setPlainText("");
    m_dialogueItem.setPlainText("");
    setPixmap(m_default);
    m_finishCondition = timeout;
    m_dialogueComplete = false;
    m_lengthOnScreen = 0;
}

void Message::hide()
{
    QGraphicsPixmapItem::hide();
    reset();
    m_dialogue.clear();
    emit hiding();
}

void Message::receiveSignal()
{
    if (m_finishCondition == Message::signal && m_dialogueComplete)
    {
        hide();
    }
}
