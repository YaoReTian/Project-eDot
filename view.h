#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include "worldscene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();

private:
    WorldScene *m_worldscene;
};

#endif // VIEW_H
