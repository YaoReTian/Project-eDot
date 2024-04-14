#include "turnbased.h"

TurnBased::TurnBased()
{
    m_background = new QGraphicsPixmapItem;
    m_friendlyTurn = false;
    m_basicButton = new Button;
    m_skillButton = new Button;

    m_basicButton->setIconText("Q");
    m_basicButton->setTriggerAction(GLOBAL::BASIC);

    m_skillButton->setIconText("E");
    m_skillButton->setTriggerAction(GLOBAL::SKILL);
}

void TurnBased::setBackground(QPixmap backgroundImg)
{
    m_background->setPixmap(backgroundImg);
}

void TurnBased::setEvent(Player* player, QList<CombatSprite*> m_enemies)
{
    for (const auto s : player->getParty())
    {
        m_spriteQueue.append(s);
    }
    for (const auto s : m_enemies)
    {
        m_spriteQueue.append(s);
    }
}

void TurnBased::removeItem(QGraphicsScene &scene)
{
    scene.removeItem(m_background);
    for (const auto s : m_spriteQueue)
    {
        s->removeItem(scene);
    }
    if (m_basicButton->isActive())
    {
        m_basicButton->removeItem(scene);
        m_skillButton->removeItem(scene);
        if (!m_friendlyTurn)
        {
            m_basicButton->setActive(false);
            m_skillButton->setActive(false);
        }
    }
}

void TurnBased::update(int deltatime, KeyMap* keys)
{

    // Sort in action value order with lambda function
    std::sort(m_spriteQueue.begin(), m_spriteQueue.end(), [](CombatSprite* a, CombatSprite* b)
              {
        return a->actionValue() < b->actionValue();
    });

    int left = 0;
    int right = 0;

    for (const auto s : m_spriteQueue)
    {
        s->setY(m_background->boundingRect().height()/2);
        if (s->side() == Friendly)
        {
            s->setX(2*m_background->boundingRect().width()/5 - left*(GLOBAL::ObjectLength + 5*GLOBAL::Scale) - s->boundingRect().width());
            left++;
        }
        else
        {
            s->setX(3*m_background->boundingRect().width()/5 + right*(GLOBAL::ObjectLength + 5*GLOBAL::Scale));
            right++;
        }
        s->update(deltatime);
    }

    if (m_spriteQueue.head()->side() == Friendly)
    {
        m_friendlyTurn = true;

        m_basicButton->setPos(m_background->boundingRect().width() - m_basicButton->boundingRect().width() - 2*GLOBAL::Scale,
                              m_background->boundingRect().height() - 2*(m_basicButton->boundingRect().height() + 2*GLOBAL::Scale));
        m_skillButton->setPos(m_background->boundingRect().width() - m_skillButton->boundingRect().width() - 2*GLOBAL::Scale,
                              m_background->boundingRect().height() - m_skillButton->boundingRect().height() - 2*GLOBAL::Scale);

        m_skillButton->setFocused();
        m_basicButton->setFocused();

        m_basicButton->update(keys);
        m_skillButton->update(keys);
    }
}

void TurnBased::render(QGraphicsScene &scene)
{
    scene.addItem(m_background);
    for (const auto s : m_spriteQueue)
    {
        s->render(scene);
    }
    if (m_friendlyTurn)
    {
        m_basicButton->render(scene);
        m_skillButton->render(scene);
        if (!m_basicButton->isActive())
        {
            m_basicButton->setActive(true);
            m_skillButton->setActive(true);
        }
    }
}
