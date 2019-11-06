#include "glass.h"
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QKeyEvent>
#include <math.h>

#define PI 3.14159265

glass::glass(QWidget *parent) : QWidget(parent)
{
    //---------------------- отложенная инициализация -------------------------------------------
       connect(this,SIGNAL(signalGlassInit()),this, SLOT(slotGlassInit()),Qt::QueuedConnection);
       emit signalGlassInit();
    //-------------------------------------------------------------------------------------------

}

void glass::slotGlassInit()
{
   //--------------- инициализация массива -------------------------------
    glassArray.resize(this->rows());
       for(int i = 0; i < this->rows(); i++)
       {
            glassArray[i].resize(this->colomns());
       }
       glassArray.fill(glassArray[0].fill(emptyCell));
}


void glass::paintEvent(QPaintEvent *event)
{
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------- рисуем сетку -----------------------------------
       QPainter painter(this);
       painter.setPen (QPen (Qt::gray, 1, Qt::SolidLine) ) ;

       for (uint i = 0; i <  this->colomns(); ++i)
        {
         for(uint j = 0; j < this->rows(); j++)
           {
           int x1 = 10*i;
           int y1 = 10*j;
           int w1 = 10;
           int h1 = 10;

       painter.setBrush(QBrush(glassArray[j][i], Qt::SolidPattern));
       painter.drawRect(x1,y1,w1,h1);
            }
          }

       //------------------------------------------------------------

       int y = this->width();
       int x = this->height();

        painter.setPen (QPen (Qt::black, 1, Qt::SolidLine) ) ;
        painter.setFont(QFont("Times", 8, QFont::Normal));
        painter.drawText(p2_x+5, x-p2_y+5, "Point 1");
        painter.drawText(p0_x+5, x-p0_y+5, "Point 2");
        painter.drawText(p1_x+5, x-p1_y+5, "Point 3");
//------------------------------------------------------------
//------------------------------------------------------------

QPointF p0(p0_y, p0_x);
QPointF p1(p1_y, p1_x);
QPointF p2(p2_y, p2_x);

QLineF l1(p0,p1);
QLineF l2(p0,p2);

//расчет угла между двумя прямыми

float cos = atan2f(p2.y()-p0.y(), p2.x() - p0.x())*180/PI;
float cos1 = atan2f(p1.y()-p0.y(), p1.x() - p0.x())*180/PI;
float res = 0;  //угол

    if ((cos1-cos)<0) res=cos1-cos+360;
    if ((cos1-cos)>0) res=cos1-cos;

 float ak = rad/tan((res/2)*PI/180); //расчет катета касательной


   QLineF angline1;
   QLineF angline2;

   angline1.setP1(p0);
   angline1.setP2(p2);
   angline1.setLength(ak);

   angline2.setP1(p0);
   angline2.setP2(p1);
   angline2.setLength(ak);

   QPointF B(angline1.p2()); // точка PTR1
   QPointF A(angline2.p2()); // точка PTR2

   pk1_y = B.x();
   pk1_x = B.y();
   pk2_y = A.x();
   pk2_x = A.y();

   painter.drawText(B.y()+5, x-B.x()+5, "PtR1");
   painter.drawText(A.y()+5, x-A.x()+5, "PtR2");

//------------------------------------------------------------
//------------------------------------------------------------

       painter.translate(0.0,this->height());
       painter.rotate(-90.0);
       painter.setPen (QPen (Qt::blue, 2, Qt::SolidLine) ) ;
       painter.setRenderHint(QPainter::Antialiasing, true);
       painter.setPen (QPen (Qt::black, 3, Qt::SolidLine) ) ;
       painter.drawLine(QLine(0,0,x,0));
       painter.drawLine(QLine(0,0,0,y));

       QVector<QLineF> clines;

       clines.resize(0);

       for (int i = 0; i < y; i+=10)
       {
             clines.push_back(QLineF(0, i, 4, i));
       }

       for (int i = 0; i < x; i+=10)
       {
            clines.push_back(QLineF(i, 0, i, 4));
       }

       for (int i = 0; i < y; i+=50)
       {
             clines.push_back(QLineF(0, i, 7, i));
             painter.drawText(10,i+5, QString::number(i));
       }

       for (int i = 0; i < x; i+=50)
       {
            clines.push_back(QLineF(i, 0, i, 7));
            painter.drawText(i-5,20, QString::number(i));
       }

       painter.setPen (QPen (Qt::black, 1, Qt::SolidLine) ) ;
       painter.setRenderHint(QPainter::Antialiasing, true);
       painter.drawLines(clines);
//-------------------------------------------------------------------------------------------------------
       painter.setPen (QPen (Qt::black, 2, Qt::SolidLine) ) ;

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------

 painter.drawEllipse(QPointF(p0),2.0, 2.0);
 painter.drawEllipse(QPointF(p1),2.0, 2.0);
 painter.drawEllipse(QPointF(p2),2.0, 2.0);
 painter.drawLine(l1); //рисуем линии по заданным точкам
 painter.drawLine(l2);
 painter.drawEllipse(QPointF(angline1.p2()),2.0, 2.0);
 painter.drawEllipse(QPointF(angline2.p2()),2.0, 2.0);

 float gip = sqrt(ak*ak + rad*rad); //гипотенуза прямоуг треугольника
 float anglgip = 0;

 if (angline1.angle() < 180) anglgip = angline1.angle()-res/2;
 if (angline1.angle() > 180) anglgip = angline1.angle()+res/2;

 QLineF line_b;  //биссектриса угла

line_b.setP1(p0);
line_b.setAngle(anglgip);
line_b.setLength(gip);

 QPointF O(line_b.p2()); //точка-центр окружности

//---- рисуем дугу, основа квадрат опис.окружность, размер дуги=углу ------------------------------

 QRectF rectangle(O.x()-rad, O.y()-rad, 2*rad, 2*rad);
 int startAngle = (anglgip+res/2+90) * 16;
 int spanAngle = (180-res) * 16;

painter.drawArc(rectangle, startAngle, spanAngle);

//---------------------- вызов слота для передачи в маин -------------------------------------------
  emit signalGlassRepaint();
//-------------------------------------------------------------------------------------------

}
