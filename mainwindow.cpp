#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_view(new View), m_keymap(new KeyMap)
{
    ui->setupUi(this);
    setCentralWidget(m_view);
    m_keymap->setWorldBindings();

    QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(loop()));
    m_timer.start(int(1000.0f/GLOBAL::FPS));
    m_elapsedTimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_view;
    delete m_keymap;
}

void MainWindow::loop()
{
    m_deltatime = m_elapsedTimer.elapsed();
    m_elapsedTimer.restart();

    m_view->input(m_keymap);
    m_view->update(m_deltatime);

    m_keymap->resetStatus();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Qt::Key key = static_cast<Qt::Key>(event->key());
    if (m_keymap->contains(key))
    {
        GLOBAL::Action action = m_keymap->getAction(key);
        m_keymap->keyHeld(action);
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat())
    {
        Qt::Key key = static_cast<Qt::Key>(event->key());

        if (m_keymap->contains(key))
        {
            GLOBAL::Action action = m_keymap->getAction(key);
            m_keymap->keyReleased(action);
        }
    }
    QMainWindow::keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    m_keymap->mouseHeld();
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_keymap->mouseReleased();
    QMainWindow::mouseReleaseEvent(event);
}
