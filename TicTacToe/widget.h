#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>

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
    int Minimax(QVector<QVector<int>>board, int layer, QString player, int a, int b, bool first);//极小化极大算法原理来得出决策
    void drawdecision();//将机器做出的决策展示出来
    int judge();//棋盘下满时判定胜负或平局
    int evaluate(QVector<QVector<int>>board,int layer);//判断当局是否已分出了胜负

    QVector<QVector<int>>chessboard;//“0”表示该格子还没被占据，“1”表示该格子被“×”占据，“2”，表示该格子被“○”占据
    const int layer=6;

public slots:
    void on_pushButton_10_clicked();//选择以“×”先手开局
    void on_pushButton_11_clicked();//选择以“○”后手开局
    void on_pushButton_12_clicked();//重新开始
    /*点击棋盘*/
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();

private:
    Ui::Widget *ui;
    bool choosex=false;
    bool chooseo=false;
    int manchoose=0;
    int machinechoose=0;
    int chessleft=9;
    int decidex=0,decidey=0;
    bool manwin=false;
    bool machinewin=false;
    bool tie=false;
};

#endif // WIDGET_H
