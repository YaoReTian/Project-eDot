#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

#include "../Utils/global.h"
#include "../UI/button.h"

struct AnimationState
{
    QString stateName;
    float frameTime;
    QList<QPixmap> frames;
    QMap<GLOBAL::Action, QString> transitions;
};

class Sprite : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Sprite(QGraphicsItem * parent =0);

    void setID(int ID);
    void setName(QString name);
    void setSpriteSheet(QString spriteSheet);
    void setType(QString type);

    void removeItem(QGraphicsScene &scene);
    void update(int deltaTime);
    void render(QGraphicsScene &scene);

    void setAction(GLOBAL::Action action);

    int getID();
    QString getName();
    QString getType();
    Button* getButton();
    QString getScript();

    // For animations
    void setSpriteSheet(QPixmap spriteSheet);
    void setFrameSize(QSize frameSize);
    void setInteraction(QString text, QString dialogue);
    void addAnimationState(QString stateName, int startFrame, int endFrame, float frameTime);
    void addTransition(QString startStateName, GLOBAL::Action triggerAction, QString endStateName);

    // For interactions with the player
    bool isInteractable();
    bool isInteractingWithPlayer();
    void popup();

private:
    // Sprite data
    int m_SpriteID;
    QString m_name;
    QString m_type;
    bool m_interactable;
    QString m_interactText;
    QString m_script;

    // For animations
    QSize m_frameSize;
    int m_elapsed_time;
    int m_currentFrame;
    QString m_currentStateName;
    QPixmap m_spriteSheet;
    QMap<QString, AnimationState*> m_states;

    // For interactions
    Button * m_button;
    bool m_interactingWithPlayer;
};

#endif // SPRITE_H
