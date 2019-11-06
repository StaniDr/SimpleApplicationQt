#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    QVBoxLayout *pBoxLayout_Main;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:

    void slotFrame();

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
