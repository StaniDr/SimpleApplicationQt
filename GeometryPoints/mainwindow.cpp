#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //---------- метод для передачи  ----------
            connect(ui->GLASS,SIGNAL(signalGlassRepaint()), this, SLOT(slotFrame()),Qt::QueuedConnection);

    createMenuBar();

     ui->GLASS->p0_x = ui->P0_X_corEdit->value();
     ui->GLASS->p0_y = ui->P0_Y_corEdit->value();
     ui->GLASS->p1_x = ui->P1_X_corEdit->value();
     ui->GLASS->p1_y = ui->P1_Y_corEdit->value();
     ui->GLASS->p2_x = ui->P2_X_corEdit->value();
     ui->GLASS->p2_y = ui->P2_Y_corEdit->value();
     ui->GLASS->rad = ui->Radius_Edit->value();
     ui->GLASS->repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotUpdateButton()
{
    ui->GLASS->p0_x = ui->P0_X_corEdit->value();
    ui->GLASS->p0_y = ui->P0_Y_corEdit->value();
    ui->GLASS->p1_x = ui->P1_X_corEdit->value();
    ui->GLASS->p1_y = ui->P1_Y_corEdit->value();
    ui->GLASS->p2_x = ui->P2_X_corEdit->value();
    ui->GLASS->p2_y = ui->P2_Y_corEdit->value();
    ui->GLASS->rad = ui->Radius_Edit->value();

    ui->GLASS->repaint();

    ui->R1_X_Spin->setValue(ui->GLASS->pk1_x);
    ui->R1_Y_Spin->setValue(ui->GLASS->pk1_y);
    ui->R2_X_Spin->setValue(ui->GLASS->pk2_x);
    ui->R2_Y_Spin->setValue(ui->GLASS->pk2_y);

}

void MainWindow::about()
{

    QMessageBox::about(this, tr("About Points"),
            tr("<p>The <b>Points</b> how to calculate points of tangents to a circle,  "
               "aentered in a corner, constructed on three points."
               "Author - Drobot Stanislav"));

}

void MainWindow::createMenuBar()
{

    QAction *quitAction = new QAction(tr("&Quit"), this);
    QAction *aboutAction = new QAction(tr("&About"), this);
    QAction *aboutQtAction = new QAction(tr("&About Qt"), this);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(quitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    connect(quitAction, &QAction::triggered, this, &MainWindow::close);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::paintEvent(QPaintEvent *event)
{

}

void MainWindow::slotFrame()
{

    ui->R1_X_Spin->setValue(ui->GLASS->pk1_x);
    ui->R1_Y_Spin->setValue(ui->GLASS->pk1_y);
    ui->R2_X_Spin->setValue(ui->GLASS->pk2_x);
    ui->R2_Y_Spin->setValue(ui->GLASS->pk2_y);
}
