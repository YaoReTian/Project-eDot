#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>

#include "gamescene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();
    ~View();

private:
    GameScene *m_gamescene;

protected:
    // Overridden functions
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // VIEW_H
