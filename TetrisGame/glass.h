#ifndef GLASS_H
#define GLASS_H

#include <QWidget>
#include <QColor>
#include <figure.h>
#include <QMessageBox>

#define emptyCell QColor(150,150,150)//темносерыйцвет
#define greyCell QColor(190,190,190) //серыйцвет


class GLASS : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(uint rows READ rows WRITE setRows)
    Q_PROPERTY(uint colomns READ colomns WRITE setColomns)

    static const uint W=20;

public:
    explicit GLASS(QWidget *parent = nullptr);

        ~GLASS();

    unsigned int m_rows = 0;
    unsigned int m_colomns = 0;
    bool gameOn = false;   //идет игра

    uint score = 0;
    uint sec_clock = 0;
    uint wcell = 11;
    uint t_i = 0;

    QTimer *tmr;
    QTimer *time_game;

    uint InitTimer = 0;
    QMessageBox *msg;
    QVector<QVector< QColor >> glassArray;
    QVector<QVector< QColor >> tempArray;

    uint timerInterval;

    Figure *next;
    Figure *cur;

    uint rows()    const{return m_rows;}
    uint colomns() const{return m_colomns;}

signals:

        void signalGlassInit();
        void signalGlassRepaint();

        int signalScore(int);       
        int signalClock(int);

public slots:
    void setRows(uint rows)      {m_rows = rows;}
    void setColomns(uint colomns){m_colomns = colomns;}
    void slotGlassInit();
    void slotGlassClear();   
    void slotNewGame();
    void slotGlassCheck();
    void slotGlassAccept();
    void slotDelFour();

    void updateTime();  //Слот для обновления времени на экране
    void updateClock(); //Слот для обновления времени на экране

    // QPaintDevice interface
protected:


    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event);
};

#endif // GLASS_H
