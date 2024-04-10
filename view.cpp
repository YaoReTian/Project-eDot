#include "view.h"

View::View() :
    m_worldscene(new WorldScene)
{
    setScene(m_worldscene);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}
