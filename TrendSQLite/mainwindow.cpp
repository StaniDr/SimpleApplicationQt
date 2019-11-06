#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointdelegate.h"
#include "initdb.h"
#include <QtSql>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(
                    this,
                    );

    QSqlError err = initDb();
    if (err.type() != QSqlError::NoError) {
        showError(err);
        return;
    }

    // Create the data model:
    model = new QSqlRelationalTableModel(ui->bookTable);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setTable("points");
    model->setHeaderData(model->fieldIndex("X_cor"), Qt::Horizontal, tr("X"));
    model->setHeaderData(model->fieldIndex("Y_cor"), Qt::Horizontal, tr("Y"));

    // Populate the model:
    if (!model->select()) {
        showError(model->lastError());
        return;
    }
    // Set the model and hide the ID column:
    ui->bookTable->setModel(model);
    ui->bookTable->setItemDelegate(new PointDelegate(ui->bookTable));
    ui->bookTable->setColumnHidden(model->fieldIndex("id"), true);
    ui->bookTable->setColumnHidden(model->fieldIndex("N_point"), true);
    ui->bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->bookTable->horizontalHeader()->setSectionResizeMode(model->fieldIndex("N_point"),
                QHeaderView::ResizeToContents);

    QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    mapper->setItemDelegate(new PointDelegate(this));
    mapper->addMapping(ui->N_pointEdit, model->fieldIndex("N_point"));
    mapper->addMapping(ui->X_corEdit, model->fieldIndex("X_cor"));
    mapper->addMapping(ui->Y_corEdit, model->fieldIndex("Y_cor"));

    connect(ui->bookTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            mapper,
            &QDataWidgetMapper::setCurrentModelIndex
            );

    ui->bookTable->setCurrentIndex(model->index(0, 0));
    createMenuBar();

 //-----------------------------------------------------------------------------------------
  QSqlQuery q;
  q.exec("SELECT * FROM points;");
  QSqlRecord rec     = q.record();

  ui->GLASS->vec.resize(0);

    while (q.next()) {

        int x = q.value(rec.indexOf("N_point")).toInt();
        int y = q.value(rec.indexOf("X_cor")).toInt();
        int z = q.value(rec.indexOf("Y_cor")).toInt();

        ui->GLASS->vec.push_back(QPointF(y,z));
    }
    ui->GLASS->repaint();
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotAddButton()
{

    QSqlQuery q;
    int X = ui->X_corEdit->value();
    int Y = ui->Y_corEdit->value();
    QModelIndex primaryKeyIndex = ui->bookTable->currentIndex();
    int id = model->rowCount();
    id++;


    if (!q.prepare(QLatin1String("insert into points(N_point, X_cor, Y_cor) values(?, ?, ?)")))
        qDebug() << "error_insert";

   addPoint(q, id,X,Y);

    // Populate the model:
    if (!model->select()) {
        showError(model->lastError());
        return;
    }

 slotVecUpdate();

}

void MainWindow::slotUpdateButton()
{
     QSqlQuery q;

     int N = ui->N_pointEdit->value();
     int X = ui->X_corEdit->value();
     int Y = ui->Y_corEdit->value();

     QModelIndex Index = ui->bookTable->currentIndex();

         q.prepare("update points set X_cor= :X_cor, Y_cor= :Y_cor where id= :N_point ;");
         q.bindValue(":N_point", N);
         q.bindValue(":X_cor", X);
         q.bindValue(":Y_cor", Y);
         q.exec();

     // Populate the model:
     if (!model->select()) {
         showError(model->lastError());
         return;
     }
     slotVecUpdate();
}

void MainWindow::slotDelButton()
{

    QSqlQuery q;

    int N = model->rowCount();

//    //удаление
    q.prepare("delete from points where id= :ID ;");
    q.bindValue(":ID", N);
    q.exec();

    if (!model->select()) {
        showError(model->lastError());
        return;
    }
     slotVecUpdate();
}

void MainWindow::slotVecUpdate()
{
     QSqlQuery q;
      q.exec("SELECT * FROM points;");
         //Reading of the data
      QSqlRecord rec     = q.record();
      ui->GLASS->vec.resize(0);

    while (q.next()) {
        int x = q.value(rec.indexOf("N_point")).toInt();
        int y = q.value(rec.indexOf("X_cor")).toInt();
        int z = q.value(rec.indexOf("Y_cor")).toInt();
         ui->GLASS->vec.push_back(QPointF(y,z));
    }

ui->GLASS->repaint();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Points"),
            tr("<p>The <b>Points</b> shows how to use graph with SQLite-database "
               "addition, removal, editing points in database."
               "Author - Drobot Stanislav"));
}

void MainWindow::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "Unable to initialize Database",
                          "Error initializing database: " + err.text());
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
