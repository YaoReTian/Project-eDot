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

private:
    int m_spriteID;
    QString m_name;
    QString m_type;
};

#endif // SPRITE_H
