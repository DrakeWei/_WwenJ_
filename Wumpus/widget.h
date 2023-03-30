#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVector>
#include <QList>

struct point{//房间结构体
    int x=0,y=0;
    bool up=false,down=false,left=false,right=false;
    point* parent;
};

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
    
    QVector<QVector<int>>map;//Wumpus世界地图
    QVector<QVector<int>>stench;//发出恶臭的房间
    QVector<QVector<int>>breeze;//有微风的房间
    QVector<QVector<int>>location;//智能体、怪物、陷阱、金子的具体位置
    QVector<QVector<bool>>visited;//房间是否探寻过
    QVector<QVector<bool>>safety;//安全的房间
    QVector<QVector<int>>doubtmonster;//每个房间标记为怀疑有怪物的次数
    QVector<QVector<int>>doubtpit;//每个房间标记为怀疑有陷阱的次数
    QVector<QVector<point*>>room;//每个房间的出发方向枚举
    QList<point*>track;
    
    void initial();//初始化世界
    void createmonster();//初始化怪物
    void createpit();//初始化陷阱
    void creategold();//初始化金子
    void goback();//回到起始点
    void onlineDFS();//联机深度优先搜索
    void clearstench();//射死了怪物，臭味清空
    void cancelmonsterdoubt();//若已经确定了怪物的具体位置则将之前被怀疑为有怪物的房间解除怀疑

public slots:
    void on_pushButton_clicked();//开始按钮
    void on_pushButton_2_clicked();//移动按钮
    void on_pushButton_3_clicked();//重置按钮
    void timeout1();

private:
    Ui::Widget *ui;
    QTimer *timer;//智能体移动计时器
    int agentx=0;//智能体实时定位
    int agenty=0;
    const int width=150;
    int goldx=0,goldy=0;//金子位置
    int score=0;//分数
    int arrow=1;//弓箭计数
    bool gameover=false;//判断游戏是否结束
    bool back=false;//判断要返回起点
    bool findwumpus=false;//判断是否找到了怪物
    bool findgold=false;//判断是否拿到了金子
    bool shootwumpus=false;//判断是否射死了怪物
    int wumpusx=0;//若找到了怪物，则给其定位
    int wumpusy=0;
};

#endif // WIDGET_H
