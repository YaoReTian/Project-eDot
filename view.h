#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "gamescene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();

private:
    GameScene m_gamescene;
};

#endif // VIEW_H
