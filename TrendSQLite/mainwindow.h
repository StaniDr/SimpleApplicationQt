#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtWidgets>
#include <QtSql>


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
    void slotAddButton();     //кнопка-добавить точку
    void slotUpdateButton();  //кнопка-обновить
    void slotDelButton();     //кнопка-удалить точку
    void slotVecUpdate();     //обновить график

private slots:
    void about();


private:
    void showError(const QSqlError &err);
    Ui::MainWindow *ui;
    QSqlRelationalTableModel *model;
    int authorIdx, genreIdx;
    void createMenuBar();

protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // MAINWINDOW_H
