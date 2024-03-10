#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

struct AnimationState
{
    QString stateName;
    float frameTime;
    QList<QPixmap> frames;
};

class Sprite : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Sprite(QGraphicsItem * parent =0);

    void setID(int ID);
    void setName(QString name);
    void setType(QString type);
    void setSpriteSheet(QString spriteSheet);
    void update(int deltaTime);

    int getID();
    QString getName();
    QString getType();

    // For animations
    void setSpriteSheet(QPixmap spriteSheet);
    void setFrameSize(QSize frameSize);
    void setInteraction(QString text, QString dialogue);
    void addAnimationState(QString stateName, int startFrame, int endFrame, float frameTime);
    void transition(QString stateName);

    bool isInteractable();
private:
    // Sprite data
    int m_SpriteID;
    QString m_name;
    QString m_type;
    bool m_interactable;
    QString m_interactText;
    QString m_interactDialogue;

    // For animations
    QSize m_frameSize;
    int m_elapsed_time;
    int m_currentStateIndex;
    int m_currentFrame;
    QPixmap m_spriteSheet;
    QList<AnimationState*> m_states;

};

#endif // SPRITE_H
