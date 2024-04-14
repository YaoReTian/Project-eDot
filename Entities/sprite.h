#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "../Utils/global.h"
#include "../gameobject.h"

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

    virtual void removeItem(QGraphicsScene &scene) override;
    virtual void update(int deltaTime) override;
    virtual void render(QGraphicsScene &scene) override;

    void setAction(GLOBAL::Action action);

    int getID();
    QString getName();
    QString getType();

    // For animations
    void setSpriteSheet(QPixmap spriteSheet);
    void setFrameSize(QSize frameSize);
    void addAnimationState(QString stateName, int startFrame, int endFrame, float frameTime);
    void addTransition(QString startStateName, GLOBAL::Action triggerAction, QString endStateName);

private:
    // Sprite data
    int m_SpriteID;
    QString m_name;
    QString m_type;

    // For animations
    QSize m_frameSize;
    int m_elapsed_time;
    int m_currentFrame;
    QString m_currentStateName;
    QPixmap m_spriteSheet;
    QMap<QString, AnimationState*> m_states;
};

#endif // SPRITE_H
