#ifndef SPRITE_H
#define SPRITE_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>

class Sprite : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    Sprite(QGraphicsItem * parent = 0);

    void setID(int ID);
    void setSpriteSheet(QPixmap spriteSheet);
    void setName(QString name);
    void setType(QString type);

    int getID();
    QString getName();
    QString getType();

private:
    int m_spriteID;
    QString m_name;
    QString m_type;
    QPixmap m_spriteSheet;
};

#endif // SPRITE_H
