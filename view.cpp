#include "view.h"

View::View() :
    m_gamescene(new GameScene)
{
    setScene(m_gamescene);
    setScene(m_gamescene);
    m_gamescene->setSceneRect(m_gamescene->sceneRect().x(),m_gamescene->sceneRect().y(),size().width(), size().height());
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

View::~View()
{
    delete m_gamescene;
}

void View::resizeEvent(QResizeEvent* event)
{
    m_gamescene->setSceneRect(m_gamescene->sceneRect().x(),m_gamescene->sceneRect().y(),
                              event->size().width(), event->size().height());
    QGraphicsView::resizeEvent(event);
}
