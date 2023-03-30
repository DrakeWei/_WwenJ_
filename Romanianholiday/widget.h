#ifndef WIDGET_H
#define WIDGET_H
#include "matrix.h"
#include <QWidget>
#include <QString>
#include <sys/time.h>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void paintEvent(QPaintEvent *);//绘图事件
    void create();
    QTimer* timer1;
    QTimer* timer0;
    QTimer* timer2;

    int times=0;
    int times0=0;
    int times2=0;

public slots:
    void on_pushButton_clicked();//代价一致宽度优先
    void on_pushButton_2_clicked();//贪婪算法
    void on_pushButton_3_clicked();//A*算法
    void on_pushButton_4_clicked();//清空
    void timeout1();
    void timeout0();
    void timeout2();

private:
    Ui::Widget *ui;
    Matrix* matrix;
};

#endif // WIDGET_H
