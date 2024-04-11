#ifndef COMBATSPRITE_H
#define COMBATSPRITE_H

#include "movingsprite.h"

struct Move
{
    QString m_moveName;
    QString m_script;
};

enum Stat
{
    HP,
    ATK,
    DEF,
    SPD,
    CritDMG,
    CritRate
};

enum MoveType
{
    Basic,
    Skill,
    Overdrive
};

enum Side
{
    Friendly,
    Enemy
};

struct StatScaler
{
    float m_percentChange;
    int m_flatChange;
};

struct StatChange
{
    QString m_name;
    QString m_desc;
    int m_duration;
    QMap<Stat, StatScaler*> m_changes;
};

class CombatSprite : public MovingSprite
{
public:
    CombatSprite();

    // Game loop functions
    void update(int deltatime);

    // Setters
    void setEntityID(int EntityID);
    void setEntityName(QString name);
    void setElement(GLOBAL::Element element);
    void setBaseStat(Stat stat, int value);
    void setStatVal(Stat stat, float percent, int bonus);
    void setEnergyCost(int value);
    void setDesc(QString description);
    void setlevel(int level);
    void setScaleStat(Stat stat);
    void setMove(MoveType movetype, QString name, QString script);
    void setSide(Side side);

    // Getters
    int getEntityID();
    QString getEntityName();
    QString getDesc();
    GLOBAL::Element getElement();
    int getBaseStat(Stat statToGet);
    int getStat(Stat statToGet);
    int getLevel();
    int getCurrentHP();
    Stat getScaleStat();
    QString getMoveScript(MoveType moveType);
    int actionValue();
    Side side();
    bool enteredCombat();

    // Manipulating stats

    // HP
    void takeDMG(int DMGtaken);
    void heal(int RecoveredHP);
    void changeStat(StatChange* change);

    // Energy
    void energyChange(int val);
    void depleteEnergy();

    // Action value
    void nextTurn(int actionValue);
    void actionForward(float percent);
    void resetActionValue();

private:
    int m_EntityID;
    int m_level;
    QString m_entityName;
    GLOBAL::Element m_element;
    QMap<Stat, int> m_baseStats;
    QMap<Stat, int> m_stats;
    QMap<Stat, StatScaler*> m_statScalers;
    QList<StatChange*> m_statChanges;
    Stat m_scaleStat;
    int m_currentHP;
    int m_maxEnergy;
    int m_energyCharged;
    QString m_description;
    QMap<MoveType, Move*> m_moveset;
    int m_actionValue;
    int m_baseActionValue;
    Side m_side;
    bool m_enteredCombat;
};

#endif // COMBATSPRITE_H
