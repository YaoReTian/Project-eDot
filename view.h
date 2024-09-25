#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QResizeEvent>
#include "Utils/keymap.h"

#include "gamescene.h"

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();
    ~View();

    void input(KeyMap* keys);
    void update(int deltatime);

private:
    GameScene *m_gamescene;

protected:
    // Overridden functions
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // VIEW_H
