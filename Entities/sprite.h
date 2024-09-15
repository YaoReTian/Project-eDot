#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "../Utils/global.h"
#include "../gameobject.h"
#include "../tileset.h"

struct AnimationState
{
    QString stateName;
    float frameTime;
    QList<QPixmap> frames;
    QMap<GLOBAL::Action, QString> transitions;
};

class Sprite : public QObject, public QGraphicsPixmapItem, public GameObject
{
    Q_OBJECT
public:
    Sprite(QGraphicsItem * parent =0);
    ~Sprite();

    void setID(int ID);
    void setName(QString name);
    void setSpriteSheet(QString spriteSheet);
    void setType(QString type);
    void setZValue(float zValue);
    void setTransform(QTransform transform);
    void setHitbox(HitboxInfo* hitbox);

    virtual void clear(QGraphicsScene &scene) override;
    virtual void update(int deltaTime) override;
    virtual void render(QGraphicsScene &scene) override;

    void setAction(GLOBAL::Action action = GLOBAL::NONE);

    int getID();
    QString getName();
    QString getType();

    // For animations
    void setSpriteSheet(QPixmap spriteSheet);
    void setFrameSize(QSize frameSize);
    void addAnimationState(QString stateName, int startFrame, int endFrame, float frameTime);
    void addTransition(QString startStateName, GLOBAL::Action triggerAction, QString endStateName);

    // Movement
    void setDefaultToWalk();
    void setDefaultToRun();
    bool collidedWithWall();

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
    const float m_WALK_SPEED;
    const float m_RUN_SPEED;
    const float m_SPRINT_SPEED;
    float m_defaultSpeed;
    float m_currentSpeed;
    float m_velocityX, m_velocityY;
    float m_baseZ;
};

#endif // SPRITE_H
