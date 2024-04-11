#include "turnbased.h"

#include <algorithm>

TurnBased::TurnBased()
{
    m_background = new QGraphicsPixmapItem;
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
}

void TurnBased::update(int deltatime)
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
}

void TurnBased::render(QGraphicsScene &scene)
{
    scene.addItem(m_background);
    for (const auto s : m_spriteQueue)
    {
        s->render(scene);
    }
}
