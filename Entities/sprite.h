#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "../Utils/global.h"
#include "gameitem.h"
#include "../tileset.h"
#include "../Utils/vector.h"

struct AnimationState
{
    QString stateName;
    float frameTime;
    QList<QPixmap> frames;
    QMap<GLOBAL::Action, QString> transitions;
};

class Sprite : public QObject, public GameItem
{
    Q_OBJECT
public:
    Sprite(QGraphicsItem * parent =0);
    ~Sprite();

    void setID(int ID);
    void setName(QString name);
    void setSpriteSheet(QString spriteSheet);
    void setType(QString type);
    void setTransform(QTransform transform);
    void setHitbox(HitboxInfo* hitbox);

    void showHitbox();
    void hideHitbox();

    virtual void update(int deltaTime) override;

    void setAction(GLOBAL::Action action = GLOBAL::NONE);

    int getID();
    QString getName();
    QString getType();
    Vector prevActiveVector();


    // For animations
    void setSpriteSheet(QPixmap spriteSheet);
    void setFrameSize(QSize frameSize);
    void addAnimationState(QString stateName, int startFrame, int endFrame, float frameTime);
    void addTransition(QString startStateName, GLOBAL::Action triggerAction, QString endStateName);
    QSize frameSize();

    // Movement
    void setDefaultToWalk();
    void setDefaultToRun();
    bool collidedWithWall();

public slots:

private:
    // Sprite data
    int m_SpriteID;
    QString m_name;
    QString m_type;
    QList<Hitbox*> m_hitboxes;

    // For animations
    QSize m_frameSize;
    int m_elapsed_time;
    int m_currentFrame;
    QString m_currentStateName;
    QPixmap m_spriteSheet;
    QMap<QString, AnimationState*> m_states;
    QTransform m_transform;

    // Movement
    Vector m_prevActiveVector;
    const float m_WALK_SPEED;
    const float m_RUN_SPEED;
    const float m_SPRINT_SPEED;
    float m_defaultSpeed;
    float m_currentSpeed;
};

#endif // SPRITE_H
