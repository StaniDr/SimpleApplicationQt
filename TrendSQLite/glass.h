#ifndef GLASS_H
#define GLASS_H
#include <QWidget>

#define emptyCell QColor(230,230,230)//серыйцвет

class glass : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(uint rows READ rows WRITE setRows)
    Q_PROPERTY(uint colomns READ colomns WRITE setColomns)

    uint m_rows;
    uint m_colomns;
    QVector<QVector< QColor >> glassArray;

public:
    explicit glass(QWidget *parent = nullptr);

    QVector<QPointF> vec;
    uint rows() const    {return m_rows;}
    uint colomns() const {return m_colomns;}

signals:

    void signalGlassInit();
    void signalGlassRepaint();


public slots:

    void slotGlassInit();
    void slotGlassClear();
    void setRows(uint rows){m_rows = rows;}
    void setColomns(uint colomns){m_colomns = colomns;}

// QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif // GLASS_H
