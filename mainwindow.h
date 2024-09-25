#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QMouseEvent>
#include <QKeyEvent>

#include "view.h"
#include "Utils/keymap.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void loop();

private:
    Ui::MainWindow *ui;
    int m_deltatime;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    View* m_view;
    KeyMap* m_keymap;

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
};

#endif // MAINWINDOW_H
