#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator; 		                    //создаем переводчик
    translator.load("trend_ru.qm",                  //имя файла  перевода
                ".");			                          //директория - текущая
    QApplication ::installTranslator(&translator); 	//применяет переводчик ко всему приложению

    MainWindow w;
    w.setFixedSize(650,420);
    w.show();

    return a.exec();
}
