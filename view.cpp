#include "view.h"

View::View() :
    m_gamescene(new GameScene)
{
    setScene(m_gamescene);
    m_gamescene->testOutput();
}
