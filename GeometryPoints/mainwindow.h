#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>


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
    void slotUpdateButton();

private slots:
    void about();
    void slotFrame();

private:

    Ui::MainWindow *ui;
    int authorIdx, genreIdx;
    void createMenuBar();

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
