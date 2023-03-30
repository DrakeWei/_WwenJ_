#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QChar>
#include <QPaintDevice>
#include <QPaintEvent>
#include <QFont>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QRect>
#include <QPen>
#include <QDebug>
#include <QVector>
#include <QTime>
#include <QTimer>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("罗马尼亚度假问题");
    setFixedSize(1112,725);
    create();
    timer1 = new QTimer(this);
    connect(this->timer1, SIGNAL(timeout()),this,SLOT(timeout1()));//贪婪算法计时器
    timer0 = new QTimer(this);
    connect(this->timer0, SIGNAL(timeout()),this,SLOT(timeout0()));//一致代价搜索计时器
    timer2 = new QTimer(this);
    connect(this->timer2, SIGNAL(timeout()),this,SLOT(timeout2()));//A*算法计时器
}

Widget::~Widget()
{
    delete ui;
}

void Widget::create(){
    matrix=new Matrix();
    matrix->createEdgeDistance();
    matrix->getvalue();
    matrix->List[0].x=80;matrix->List[0].y=320;
    matrix->List[1].x=690;matrix->List[1].y=520;
    matrix->List[2].x=420;matrix->List[2].y=610;
    matrix->List[3].x=250;matrix->List[3].y=600;
    matrix->List[4].x=1050;matrix->List[4].y=610;
    matrix->List[5].x=500;matrix->List[5].y=320;
    matrix->List[6].x=650;matrix->List[6].y=620;
    matrix->List[7].x=950;matrix->List[7].y=480;
    matrix->List[8].x=770;matrix->List[8].y=200;
    matrix->List[9].x=240;matrix->List[9].y=470;
    matrix->List[10].x=250;matrix->List[10].y=530;
    matrix->List[11].x=610;matrix->List[11].y=150;
    matrix->List[12].x=160;matrix->List[12].y=170;
    matrix->List[13].x=560;matrix->List[13].y=480;
    matrix->List[14].x=370;matrix->List[14].y=410;
    matrix->List[15].x=320;matrix->List[15].y=340;
    matrix->List[16].x=100;matrix->List[16].y=420;
    matrix->List[17].x=780;matrix->List[17].y=480;
    matrix->List[18].x=870;matrix->List[18].y=320;
    matrix->List[19].x=110;matrix->List[19].y=240;
}

void Widget::timeout0(){//动态显示槽函数
    if(times0!=matrix->consider.size()){
        times0++;
    }
}

void Widget::timeout1(){//动态显示槽函数
    if(times!=matrix->show.size()){
        times++;
    }
}

void Widget::timeout2(){//动态显示槽函数
    if(times2!=matrix->considerA.size()){
        times2++;
    }
}

void Widget::paintEvent(QPaintEvent *){//绘画事件
    QPainter painter(this);
    QPen blackpen(Qt::black);//用来画初始图，以及未遍历到的结点和边
    QPen mediumgreen(QColor("#800080"),5);//用来画走过的边
    QPen greenpen(Qt::green,5);//用来画每一步的所有试探边
    QPen bluepen(QColor("#000080"));//用来写遍历过的结点名字
    QFont font;//显示最优总代价
    painter.setBrush(Qt::white);
    painter.setRenderHint(QPainter::Antialiasing,true);
    for(int i=0;i<matrix->CitySize;i++){//画初始图的边和顶点
        QRect rect1(matrix->List[i].x,matrix->List[i].y,50,50);
        for(int j=0;j<matrix->CitySize;j++){
            if(matrix->EdgeDistance[i][j]>0&&matrix->EdgeDistance[i][j]<matrix->maxdistance){
               QRect rect2(matrix->List[j].x,matrix->List[j].y,50,50);
               painter.drawLine(rect1.center(),rect2.center());
               painter.drawEllipse(rect1.center(),10,10);
               painter.drawEllipse(rect2.center(),10,10);
            }
        }
    }
    //----------------------------------*贪婪算法可视化界面*-------------------------------------//
    if(times){
        for(int i=0;i<times;i++){
            if(times==matrix->show.size()&&i==times-1){
                break;
            }
            int pos=matrix->getVertexPos(matrix->show[i].name);
            QRect rectS(matrix->List[pos].x,matrix->List[pos].y,50,50);
            for(int j=0;j<matrix->CitySize;j++){
                if(matrix->EdgeDistance[pos][j]>0&&matrix->EdgeDistance[pos][j]<matrix->maxdistance){
                    QRect rectT(matrix->List[j].x,matrix->List[j].y,50,50);
                    painter.setPen(mediumgreen);
                    painter.drawLine(rectS.center(),rectT.center());
                    painter.setPen(blackpen);
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(rectT.center(),10,10);
                }
            }
        }
        for(int i=0;i<times;i++){
            int pos=matrix->getVertexPos(matrix->show[i].name);
            QRect rectD(matrix->List[pos].x,matrix->List[pos].y,50,50);
            painter.setBrush(QColor("#afeeee"));
            painter.drawEllipse(rectD.center(),10,10);
        }
        if(times==matrix->show.size()){
            painter.setPen(blackpen);
            font.setPixelSize(20);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(920,75,QString::number(matrix->totdistance));
            painter.drawText(920,110,QString::number(matrix->exploreB));
        }
    }
    //----------------------------------*一致代价搜索可视化界面*-------------------------------------//
    if(times0){
        for(int i=0;i<times0;i++){
            if(times0==matrix->consider.size()&&i==times0-1){
                break;
            }
            int pos=matrix->getVertexPos(matrix->consider[i].name);
            QRect rectS(matrix->List[pos].x,matrix->List[pos].y,50,50);
            for(int j=0;j<matrix->CitySize;j++){
                if(matrix->EdgeDistance[pos][j]>0&&matrix->EdgeDistance[pos][j]<matrix->maxdistance){
                    QRect rectT(matrix->List[j].x,matrix->List[j].y,50,50);
                    painter.setPen(mediumgreen);
                    painter.drawLine(rectS.center(),rectT.center());
                    painter.setPen(blackpen);
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(rectT.center(),10,10);
                }
            }
        }
        node drawnode=matrix->consider[times0-1];
        while(drawnode.pos!=matrix->consider[0].pos){
            QRect rectD(drawnode.x,drawnode.y,50,50);
            painter.setBrush(QColor("#afeeee"));
            painter.drawEllipse(rectD.center(),10,10);
            drawnode=matrix->List[matrix->path[drawnode.pos]];
        }
        QRect rectD(drawnode.x,drawnode.y,50,50);
        painter.drawEllipse(rectD.center(),10,10);
        if(times0==matrix->consider.size()){
            painter.setPen(blackpen);
            font.setPixelSize(20);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(920,75,QString::number(matrix->totdistance));
            painter.drawText(920,110,QString::number(matrix->exploreA));
        }
    }
    //----------------------------------*A*算法可视化界面*-------------------------------------//
    if(times2){
        for(int i=0;i<times2;i++){
            if(times2==matrix->considerA.size()&&i==times2-1){
                int pos=matrix->getVertexPos(matrix->considerA[i].name);
                matrix->included[pos]=true;
                break;
            }
            int pos=matrix->getVertexPos(matrix->considerA[i].name);
            matrix->included[pos]=true;
            QRect rectS(matrix->List[pos].x,matrix->List[pos].y,50,50);
            for(int j=0;j<matrix->CitySize;j++){
                if(matrix->EdgeDistance[pos][j]>0&&matrix->EdgeDistance[pos][j]<matrix->maxdistance){
                    QRect rectT(matrix->List[j].x,matrix->List[j].y,50,50);
                    painter.setPen(mediumgreen);
                    painter.drawLine(rectS.center(),rectT.center());
                    painter.setPen(blackpen);
                    painter.setBrush(Qt::white);
                    painter.drawEllipse(rectT.center(),10,10);
                }
            }
        }
        painter.setBrush(QColor("#afeeee"));
        for(int i=0;i<matrix->CitySize;i++){
            if(matrix->included[i]){
                node drawnode=matrix->List[i];
                QRect rectE(drawnode.x,drawnode.y,50,50);
                painter.drawEllipse(rectE.center(),10,10);
            }
        }
        painter.setBrush(QColor("#ff4500"));
        node trynode=matrix->considerA[times2-1];
        int index=matrix->considerA[0].pos;
        while(trynode.pos!=index){
            QRect rectE(trynode.x,trynode.y,50,50);
            painter.drawEllipse(rectE.center(),10,10);
            trynode=matrix->considerA[trynode.parent];
        }
        QRect rectE(trynode.x,trynode.y,50,50);
        painter.drawEllipse(rectE.center(),10,10);
        if(times2==matrix->considerA.size()){
            painter.setPen(blackpen);
            font.setPixelSize(20);
            font.setBold(true);
            painter.setFont(font);
            painter.drawText(920,75,QString::number(matrix->totdistance));
            painter.drawText(920,110,QString::number(matrix->exploreC));
        }
    }
    update();
}

void Widget::on_pushButton_clicked(){//一致代价搜索触发按钮
    timer0->start(1000);
    QString s1=ui->lineEdit->text();
    QString s2=ui->lineEdit_2->text();
    int start=matrix->getVertexPos(s1);
    int end=matrix->getVertexPos(s2);
    node root;
    root.name=s1;
    root.pos=start;
    root.x=matrix->List[start].x;
    root.y=matrix->List[start].y;
    root.dist=0;
    matrix->path[start]=start;
    matrix->priority.append(matrix->List[start]);
    matrix->visited[start]=true;
    matrix->UCS(start,end);
    update();
}

void Widget::on_pushButton_2_clicked(){//贪婪算法触发按钮
    timer1->start(1000);
    QString s1=ui->lineEdit->text();
    QString s2=ui->lineEdit_2->text();
    int start=matrix->getVertexPos(s1);
    int end=matrix->getVertexPos(s2);
    matrix->greedysearch(start,end);
    update();//更新界面
}

void Widget::on_pushButton_3_clicked(){//A*算法触发按钮
    timer2->start(1000);
    QString s1=ui->lineEdit->text();
    QString s2=ui->lineEdit_2->text();
    int start=matrix->getVertexPos(s1);
    int end=matrix->getVertexPos(s2);
    node root;
    root.name=s1;
    root.pos=start;
    root.x=matrix->List[start].x;
    root.y=matrix->List[start].y;
    root.g=0;
    root.parent=0;
    matrix->priority.append(root);
    matrix->visited[start]=true;
    matrix->A_star(start,end);
    update();
}

void Widget::on_pushButton_4_clicked(){//清空重置触发按钮
    matrix->show.clear();
    matrix->consider.clear();
    matrix->considerA.clear();
    matrix->priority.clear();
    matrix->totdistance=0;
    matrix->exploreA=1;
    matrix->exploreB=1;
    matrix->exploreC=1;
    times=0;
    times0=0;
    times2=0;
    for(int i=0;i<matrix->CitySize;i++){
        matrix->visited[i]=false;
        matrix->included[i]=false;
        matrix->path[i]=0;
    }
}
