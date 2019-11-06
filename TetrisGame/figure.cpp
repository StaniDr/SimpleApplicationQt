#include "figure.h"
#include "glass.h"
#include <QDebug>
#include <random>
#include <QRandomGenerator>

Figure::Figure()
{
         FigColorArray.resize(3);
         FigColorArray.fill(emptyCell);
}

Figure::Figure(QColor c1, QColor c2, QColor c3)
{
        FigColorArray.resize(3);
        FigColorArray[0] = c1;
        FigColorArray[1] = c2;
        FigColorArray[2] = c3;
}

//#######################################################################################################
void Figure::rotateColors(bool b)
{
//------------ временный массив для изменения цветов --------------
     QVector <QColor> Temp;
     Temp.resize(3);

     if(b)   //--------- по часовой стрелке
     {
     Temp[0] = FigColorArray[1];
     Temp[1] = FigColorArray[2];
     Temp[2] = FigColorArray[0];
     }
     else   //--------- против часовой стрелки
     {
         Temp[0] = FigColorArray[2];
         Temp[1] = FigColorArray[0];
         Temp[2] = FigColorArray[1];
     }

     FigColorArray = Temp;   //назначаем массив
}

//#######################################################################################################
void Figure::MakeRandomColors()
{

 //------------  массив для рандомных цветов 4 цвета --------------
     QVector <QColor> arrColor;
     arrColor.resize(4);

     arrColor[0] = QColor(Qt::blue);
     arrColor[1] = QColor(Qt::green);
     arrColor[2] = QColor(Qt::red);
     arrColor[3] = QColor(Qt::yellow);

   for (int i = 0; i<3; i++)
     {
      int w = QRandomGenerator::global()->bounded(4);  //ограничение
     FigColorArray[i] = arrColor[w];
   }

}

//#######################################################################################################
void Figure::paintFigure(QPainter &painter)
{
 //------------  рисование фигурки на сетке  --------------

    painter.setPen (QPen (Qt::black, 1, Qt::SolidLine) ) ;

      for(uint i = m_j; i < (m_j+3); ++i)    //3 квадрата
        {
        int x1 = wcell *m_i;
        int y1 = wcell*(m_j + (i-m_j));
        int w1 = wcell;
        int h1 = wcell;

    painter.setBrush(QBrush(this->FigColorArray[i-m_j], Qt::SolidPattern)); //назначаем цвета текущие
    painter.drawRect(x1,y1,w1,h1);
         }
}
