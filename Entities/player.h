#ifndef PLAYER_H
#define PLAYER_H

#include <QList>
#include "sprite.h"
#include "bulletmanager.h"
#include "../Utils/keymap.h"
#include "../UI/progressbar.h"
#include "../UI/message.h"

class Player : public Sprite
{
    Q_OBJECT
public:
    Player(QGraphicsItem * parent = 0);
    ~Player();

    void setParentItem(QGraphicsItem *parent);
    void input(KeyMap* keys);
    virtual void update(int deltaTime) override;
    void setBulletManager(BulletManager* manager);

    void setHitboxPixmap(QPixmap p);

    void heal(int hpToAdd = 0);
    void reset();
    void takeHit();
    int HP() const;
    QGraphicsItem* hitboxItem();
    void graze();
    void setCharge(int val);
    int charge() const;
    int maxCharge() const;

    void setUsername(QString name);
    QString username();

signals:
    void message(QString dialogue, Message::FinishCondition condition, QString name = "");
    void dead();
    void overdriveCharged();
    void moved();
    void slowMoved();
    void shooting();
    void overdriveUsed();

public slots:
    void unfreeze();
    void freeze();
    void setTutorialMode(bool value = true);
    void nextTutorial();

private:
    QString m_username;
    int m_HP;
    int m_maxCharge;
    int m_charge;
    int m_elapsedTime;
    bool m_hitboxVisible;
    BulletManager* m_bulletManager;
    QList<VectorField*> m_fields;
    QGraphicsPixmapItem m_hitbox;

    ProgressBar* m_chargeBar;
    ProgressBar* m_healthBar;

    bool m_immune;
    int m_immuneTime;
    int m_totalImmuneTime;

    bool m_frozen;
    bool m_tutorialMode;
    int m_tutorialNum;
};

#endif // PLAYER_H
