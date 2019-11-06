#ifndef FIGURE_H
#define FIGURE_H

#include <QVector>
#include <QColor>
#include <QPainter>

class Figure
{

public:
    Figure();
    Figure(QColor c1, QColor c2, QColor c3);
    QVector <QColor> FigColorArray;
    unsigned int m_i = 0;
    unsigned int m_j = 0;

    void set_i(uint i){ m_i=i; }
    void set_j(uint j){  m_j = j; }

    uint get_i() {return m_i;}
    uint get_j(){  return m_j; }
    uint wcell = 10;

    void rotateColors(bool);                 //перемешивание цветов фигурки;
    void MakeRandomColors();                 //формирование случайных цветов
    void paintFigure(QPainter&painter);      //отрисовка фигурки
};

#endif // FIGURE_H
