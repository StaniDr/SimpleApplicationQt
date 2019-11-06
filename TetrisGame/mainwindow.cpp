#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QToolTip>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QString>
#include <QPoint>
#include <QSize>
#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolTip>
#include <QInputEvent>
#include <QtCore/QtMath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     //----------метод прорисовки фрейма со след фигуой----------
             connect(ui->glass,SIGNAL(signalGlassRepaint()), this, SLOT(slotFrame()),Qt::QueuedConnection);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotFrame()
{

    GLASS *pGlass = new GLASS;

        pGlass->setRows(5);
        pGlass->setColomns(5);
        pGlass->slotGlassInit();
        pGlass->slotNewGame();
        pGlass->cur->FigColorArray = ui->glass->next->FigColorArray;
        pGlass->cur->set_i(2);
        pGlass->cur->set_j(1);
        pGlass->repaint();

     delete ui->MyFrame->layout();
     pBoxLayout_Main = new QVBoxLayout;

     pBoxLayout_Main->addWidget(pGlass);
    ui->MyFrame->setLayout(pBoxLayout_Main);
    ui->MyFrame->repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{

}
