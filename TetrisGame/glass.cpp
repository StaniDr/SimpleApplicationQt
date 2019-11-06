#include "glass.h"
#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QColor>
#include <QKeyEvent>
#include <QTimer>

GLASS::GLASS(QWidget *parent) : QWidget(parent)
{

//---------------------- отложенная инициализация -------------------------------------------
   connect(this,SIGNAL(signalGlassInit()),this, SLOT(slotGlassInit()),Qt::QueuedConnection);
   emit signalGlassInit();
//-------------------------------------------------------------------------------------------

   msg  = new QMessageBox;
   cur  = new Figure();
   next = new Figure();

   cur->wcell = wcell;
   next->wcell = wcell;

   //таймер для опускания фигурок
    tmr = new QTimer;
    tmr->setInterval(500);
    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime()));

    //таймер для опускания фигурок
    time_game = new QTimer;
    time_game->setInterval(1000);
    connect(time_game, SIGNAL(timeout()), this, SLOT(updateClock()));
}

//#######################################################################################################

GLASS::~GLASS()
{
    delete cur;
    delete next;
}

//#######################################################################################################
//----  метод инициализации массива сетки ---------
void GLASS::slotGlassInit()
{

//--------------- инициализация массива -------------------------------
glassArray.resize(this->rows());
   for(int i = 0; i < this->rows(); i++)
     {
        glassArray[i].resize(this->colomns());
      }
   this->slotGlassClear();
}

//#######################################################################################################
//----  метод заполнения серыми квадратами массива сетки ---------
void GLASS::slotGlassClear()
{
    glassArray.fill(glassArray[0].fill(emptyCell));
    score = 0;
        this->signalScore(score);
}

//#######################################################################################################
//----  метод запуска новой игры ---------
void GLASS::slotNewGame()
{
    gameOn = true;
    this->slotGlassClear();

    //-- создаем новую фигурку ----------
    cur  = new Figure(QColor(Qt::red), QColor(Qt::green), QColor(Qt::red));
    cur->MakeRandomColors();
    cur->set_i(10);
    cur->set_j(2);
    cur->wcell = wcell;

    //-- создаем следующую фигурку ----------
    next = new Figure(QColor(Qt::black), QColor(Qt::black), QColor(Qt::black));
    next->MakeRandomColors();
    next->set_i(10);
    next->set_j(2);
    next->wcell = wcell;

    this->repaint();
    emit signalGlassRepaint(); // метод прорисовки фрейма для следующей фигурки

     tmr->start();   //пользовательский таймер
     time_game->start();
     sec_clock = 30;
}

//#######################################################################################################
//----  метод прорисовки новой сетки с добавленной фигуркой ---------

void GLASS::slotGlassCheck()
{
    for (uint i = 0; i <  this->colomns(); ++i)
     {
      for(uint j = 0; j < this->rows(); j++)
        {
        if ((i == cur->get_i()) & (j == cur->get_j()))
         {
            //добавляем новые цветные квадратики - 3 шт
            for (int k = 0; k<3; k++)
            {
             glassArray[j+k][i] = cur->FigColorArray[k];
            }
            j=j+2;
        }
       }
     }

    this->repaint();

    //меняем местами текущую со следущей фигуркой
      Figure *temp = cur;   
      cur = next;

     //позиционируем слудующую
      next = temp;
      next->MakeRandomColors();
      next->set_i(10);
      next->set_j(2);

      //---------------------- вызов слота для прорисовывания след фигуры -------------------------------------------
        emit signalGlassRepaint();
      //-------------------------------------------------------------------------------------------

      slotDelFour();
}

//#######################################################################################################
//----  метод вызова при заполнении стакана ---------

void GLASS::slotGlassAccept()
{
    if (this->cur->get_j() < 5)
    {
       score = 998;
       tmr->stop();
       msg->information(this, "Warning", "GAME OVER!");
       time_game->stop();
    }
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""

void GLASS::slotDelFour()
{
    uint temp_i = 0;
    uint temp_j = 0;
    bool rowFull = false;  // бит --------------------- запуска удаления
    bool colFull = false;

    for (uint i = 0; i <  this->colomns(); ++i)
     {
      for(uint j = 0; j < this->rows(); j++)
        {


         int  iRowFull = 0;
         int  iColFull = 0;

         QColor Qtemp  = glassArray[j][i];   //запоминаем цвет;;
//анализ горизонтального ряда
        if ((glassArray[j][i] != emptyCell) && ((i+3) < this->colomns()))
         {
            //проверяем следую четыре в ряд
            for (int k = 0; k<4; k++)
            {
                 if (glassArray[j][i+k] == Qtemp)
                 {

                     iRowFull++;
                 };

                 if (iRowFull >= 4)
                 {
                     rowFull = true;
                     temp_i = i;
                     temp_j = j;
                 };
            }
        }


 //анализ вертикальной колонки
        if ((glassArray[j][i] != emptyCell) && ((j+3) < this->rows()))
            {
                for (int k = 0; k<4; k++)
                {
                     if (glassArray[j+k][i] == Qtemp)
                     {
                      iColFull++;
                     };

                     if (iColFull >= 4)
                     {
                         colFull = true;
                         qDebug() << i << "; " << j;
                        temp_i = i;
                        temp_j = j;
                     };
                }
            }
       }
     }

    //-------------------------------------------------------

   if (rowFull)  //флаг уничтожения ряда, перерисовки стакана
   {
       for (uint i1 = temp_i; i1 <  (temp_i+4); ++i1)
        {
           for(uint j1 = temp_j; j1 > 0 ; --j1)
           {
             glassArray[j1][i1] = glassArray[j1-1][i1];
          }
        }
       score++;
       this->signalScore(score);
   }

   //-------------------------------------------------------

  if (colFull)  //флаг уничтожения колонки, перерисовки стакана
  {
          for(uint j1 = (temp_j+3); j1 > 4 ; --j1)
         {
             glassArray[j1][temp_i] = glassArray[j1-4][temp_i];
         }
      score++;
      this->signalScore(score);
  }
  //------------------------------------------------------
    this->repaint();
}

//""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
void GLASS::updateClock()
{
    sec_clock--;
    this->signalClock(sec_clock);


    if (sec_clock <= 0)
    {
       score = 998;
       tmr->stop();
       msg->information(this, "Warning", "GAME OVER!");
       time_game->stop();
    }
}

//#######################################################################################################
//----  вызов таймера опускания квадратиков ---------

void GLASS::updateTime()
{
uint j = this->cur->get_j();
uint i = this->cur->get_i();

  if (j>=2)
   {
    //----------------
    if (j>=27){
      this->cur->set_j(27);
      j = 28;
    }
    else {
      this->cur->set_j(++j);
    }
    //----------------

//анализируем чем заполенен квадратик
   if ((j>=27) & (glassArray[27][i] == emptyCell)) //если не занят на дне - прорисовка
   {
    this->slotGlassCheck();  //вызов слота прорисовки
   }
     else if (glassArray[j+3][i] != emptyCell)    //если занят - останов и прорисовка
     {
     this->slotGlassAccept();
     this->slotGlassCheck();
     }
     this->repaint();
   }
}

//#######################################################################################################
//----   ---------

void GLASS::paintEvent(QPaintEvent *event)
{
 //-------------------- рисуем сетку -----------------------------------
    QPainter painter(this);
    painter.setPen (QPen (Qt::black, 1, Qt::SolidLine) ) ;

    for (uint i = 0; i <  this->colomns(); ++i)
     {
      for(uint j = 0; j < this->rows(); j++)
        {
        int x1 = wcell*i;
        int y1 = wcell*j;
        int w1 = wcell;
        int h1 = wcell;

    painter.setBrush(QBrush(glassArray[j][i], Qt::SolidPattern));
    painter.drawRect(x1,y1,w1,h1);
         }
       }

   if(gameOn)
    {
        cur->paintFigure(painter);
    }

}
//#######################################################################################################
//----

void GLASS::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left: //------------------------- влевo --------------------------------------------
    {
        uint i = this->cur->get_i();
        uint j = this->cur->get_j();

        //--- проверка на выход за левую стенку
        if (i<=0){
          this->cur->set_i(0);
          i = 0;
        }
        else if (glassArray[j+3][i-1] == emptyCell)
        {
            this->cur->set_i(--i);
        }

     }
        break;

    case Qt::Key_Right: //------------------------- вправо --------------------------------------------
    {
                uint i = this->cur->get_i();
                uint j = this->cur->get_j();

                if (i>=19){
                  this->cur->set_i(19);
                  i = 20;
                }
                else if (glassArray[j+3][i+1] == emptyCell)
                {
                  this->cur->set_i(++i);
                }
    }
        break;

    case Qt::Key_Up: //------------------------- вверх --------------------------------------------
    {
          this->cur->rotateColors(true);
    }

       break;

    case Qt::Key_Down: //------------------------- вниз --------------------------------------------
    {
         // this->cur->rotateColors(false);
        uint j = this->cur->get_j();
        uint i = this->cur->get_i();

        if (j>=27){
          this->cur->set_j(27);
          j = 28;
        }
        else {
          this->cur->set_j(++j);
        }

                if ((j>=27) & (glassArray[27][i] == emptyCell))
                {
                    this->slotGlassCheck();  //вызов слота прорисовки
                }
                 else if (glassArray[j+3][i] != emptyCell)
                {
                     this->slotGlassAccept();
                     this->slotGlassCheck();
                 }
    }

       break;

    case Qt::Key_Space: //------------------------- ронять --------------------------------------------
    {
        uint j = this->cur->get_j();
        uint i = this->cur->get_i();

        if (j>=27){
          this->cur->set_j(27);
          j = 28;
        }
        else {
          this->cur->set_j(++j);
        }

                if ((j>=27) & (glassArray[27][i] == emptyCell))
                {
                    this->slotGlassCheck();  //вызов слота прорисовки
                }
                 else if (glassArray[j+3][i] != emptyCell)
                {
                     this->slotGlassAccept();
                     this->slotGlassCheck();
                 }

    }

    break;

    default:
        break;
    }

  this->repaint();
}


void GLASS::timerEvent(QTimerEvent *event)
{


}
