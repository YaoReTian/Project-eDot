#include "view.h"

View::View() :
    m_gamescene(new GameScene)
{
    setWindowTitle(tr("Project: eDot"));
    setScene(m_gamescene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

View::~View()
{
    delete m_gamescene;
}

void View::input(KeyMap* keys)
{
    keys->setMousePos(mapToScene(mapFromGlobal(QCursor::pos())));
    m_gamescene->input(keys);
}

void View::update(int deltatime)
{
    m_gamescene->update(deltatime);
}

void View::resizeEvent(QResizeEvent* event)
{

    m_gamescene->setSceneRect(m_gamescene->sceneRect().x(),m_gamescene->sceneRect().y(),
                              event->size().width(), event->size().height());
    QGraphicsView::resizeEvent(event);
}
