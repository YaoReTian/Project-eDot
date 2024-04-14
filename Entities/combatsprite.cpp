#include "combatsprite.h"

CombatSprite::CombatSprite() : MovingSprite()
{
    m_side = Enemy;
    m_enteredCombat = true;
    m_leftCombat = false;
    m_inCombat = false;
    m_healthBar = new ProgressBar;
    m_energyBar = new ProgressBar;

    m_stats[HP] = 5768;
    m_currentHP = 4321;

    m_maxEnergy = 100;
    m_energyCharged = 43;

    m_healthBar->setBackgroundColour(Qt::darkGray);
    m_healthBar->setOutlineColour(Qt::black);
    m_healthBar->setProgressColour(Qt::darkGreen);
    m_healthBar->setProgressLowColour(Qt::red);
    m_healthBar->setLowPercent(20);
    m_healthBar->setWidth(4*GLOBAL::Scale);
    m_healthBar->setLength(GLOBAL::ObjectLength);
    m_healthBar->setZValue(GLOBAL::RENDER_LAYER);

    m_energyBar->setBackgroundColour(Qt::darkGray);
    m_energyBar->setOutlineColour(Qt::black);
    m_energyBar->setWidth(4*GLOBAL::Scale);
    m_energyBar->setLength(GLOBAL::ObjectLength);
    m_energyBar->setZValue(GLOBAL::RENDER_LAYER);
    m_energyBar->setAllignment(Vertical);
}


/*ISSUEs
 * items removed despite not being on scene
 */
void CombatSprite::removeItem(QGraphicsScene &scene)
{
    MovingSprite::removeItem(scene);
    if (m_enteredCombat)
    {
        m_inCombat = true;
        m_enteredCombat = false;
    }
    else if (m_inCombat)
    {
        m_healthBar->removeItem(scene);
        m_energyBar->removeItem(scene);
        if (m_leftCombat)
        {
            m_inCombat = false;
            m_leftCombat = false;
        }
    }
}

void CombatSprite::update(int deltatime)
{
    m_baseActionValue = 1000/m_baseStats[SPD];
    m_actionValue = m_baseActionValue;

    if (m_inCombat)
    {
        m_healthBar->setMaximum(m_stats[HP]);
        m_healthBar->setValue(m_currentHP);
        m_energyBar->setMaximum(m_maxEnergy);
        m_energyBar->setValue(m_energyCharged);

        m_energyBar->setPos(x() - 2*GLOBAL::Scale - m_energyBar->getWidth(), y());
        m_healthBar->setPos(x() - (GLOBAL::ObjectLength - boundingRect().width())/2,
                            y() - 2*GLOBAL::Scale - m_healthBar->getWidth());
        m_energyBar->update(deltatime);
        m_healthBar->update(deltatime);
    }

    MovingSprite::update(deltatime);
}

void CombatSprite::render(QGraphicsScene &scene)
{
    MovingSprite::render(scene);
    if (m_inCombat)
    {
        m_healthBar->render(scene);
        m_energyBar->render(scene);
    }
}

// SETTERS
void CombatSprite::setEntityID(int EntityID)
{
    m_EntityID = EntityID;
}

void CombatSprite::setEntityName(QString name)
{
    m_entityName = name;
}

void CombatSprite::setElement(GLOBAL::Element element)
{
    m_element = element;
    switch (element)
    {
    case GLOBAL::PHYSICAL:
        m_energyBar->setProgressColour(Qt::darkCyan);
        break;
    case GLOBAL::QUANTUM:
        m_energyBar->setProgressColour(Qt::darkMagenta);
        break;
    case GLOBAL::IMAGINARY:
        m_energyBar->setProgressColour(QColor(255,215,00));
        break;
    }
}

void CombatSprite::setBaseStat(Stat stat, int value)
{
    m_baseStats[stat] = value;
}

void CombatSprite::setStatVal(Stat stat, float percent, int bonus)
{
    m_statScalers[stat]->m_percentChange = percent;
    m_statScalers[stat]->m_flatChange = bonus;
}

void CombatSprite::setEnergyCost(int value)
{
    m_maxEnergy = value;
}

void CombatSprite::setDesc(QString description)
{
    m_description = description;
}

void CombatSprite::setlevel(int level)
{
    m_level = level;
}

void CombatSprite::setScaleStat(Stat stat)
{
    m_scaleStat = stat;
}

void CombatSprite::setMove(MoveType movetype, QString name, QString script)
{
    m_moveset[movetype]->m_moveName = name;
    m_moveset[movetype]->m_script = script;
}

void CombatSprite::setSide(Side side)
{
    m_side = side;
}


// GETTERS
int CombatSprite::getEntityID()
{
    return m_EntityID;
}

QString CombatSprite::getEntityName()
{
    return m_entityName;
}

GLOBAL::Element CombatSprite::getElement()
{
    return m_element;
}

int CombatSprite::getBaseStat(Stat statToGet)
{
    return m_baseStats[statToGet];
}

int CombatSprite::getStat(Stat statToGet)
{
    return m_stats[statToGet];
}

int CombatSprite::getCurrentHP()
{
    return m_currentHP;
}

QString CombatSprite::getDesc()
{
    return m_description;
}

int CombatSprite::getLevel()
{
    return m_level;
}

Stat CombatSprite::getScaleStat()
{
    return m_scaleStat;
}

QString CombatSprite::getMoveScript(MoveType moveType)
{
    return m_moveset[moveType]->m_script;
}

Side CombatSprite::side()
{
    return m_side;
}

bool CombatSprite::enteredCombat()
{
    return m_enteredCombat;
}

int CombatSprite::actionValue()
{
    return m_actionValue;
}

void CombatSprite::takeDMG(int DMGtaken)
{
    m_currentHP -= DMGtaken;
    if (m_currentHP < 0)    m_currentHP = 0;
}
void CombatSprite::heal(int RecoveredHP)
{
    if (m_currentHP > 0)
    {
        m_currentHP += RecoveredHP;
        if (m_currentHP > m_stats[HP]) m_currentHP = m_stats[HP];
    }
}
void CombatSprite::changeStat(StatChange* change)
{
    m_statChanges.append(change);
    for (auto [key, value] : change->m_changes.asKeyValueRange())
    {
        m_statScalers[key]->m_flatChange = value->m_flatChange;
        m_statScalers[key]->m_percentChange = value->m_percentChange;
    }
}

void CombatSprite::energyChange(int val)
{
    m_energyCharged += val;
    if (m_energyCharged > m_maxEnergy)  m_energyCharged = m_maxEnergy;
}

void CombatSprite::depleteEnergy()
{
    m_energyCharged = 0;
}

void CombatSprite::nextTurn(int actionValue)
{
    m_actionValue -= actionValue;
}

void CombatSprite::actionForward(float percent)
{
    m_actionValue = std::max(0.0f,m_actionValue - m_baseActionValue * (percent/100));
}

void CombatSprite::resetActionValue()
{
    m_actionValue = m_baseActionValue;
}
