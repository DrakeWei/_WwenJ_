#include "widget.h"
#include "ui_widget.h"
#include <QPaintDevice>
#include <QPaintEvent>
#include <QFont>
#include <QPainter>
#include <QColor>
#include <QBrush>
#include <QRect>
#include <QPen>
#include <QTimer>
#include <QTime>
#include <QPixmap>
#include <QString>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Wumpus World");
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));//设置随机数种子，没有用固定值来作为种子，函数在每次调用(间隔大于1秒)时生成的随机数不一样
    timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()),this,SLOT(timeout1()));
    initial();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::initial(){//初始化、重置wumpus世界
    map.clear();location.clear();stench.clear();breeze.clear();doubtmonster.clear();doubtpit.clear();visited.clear();safety.clear();room.clear();track.clear();//容器类
    agentx=0;agenty=0;goldx=0,goldy=0;score=0;arrow=1;gameover=false;back=false;findwumpus=false;findgold=false;shootwumpus=false;wumpusx=0;wumpusy=0;//参数类
    for(int i=0;i<4;i++){//初始化容器
        QVector<int>v;
        QVector<bool>v2;
        QVector<bool>v3;
        QVector<point*>v4;
        for(int j=0;j<4;j++){
            v.append(0);
            v2.append(false);
            v3.append(true);
            point* newpoint=new point();
            newpoint->x=i;
            newpoint->y=j;
            v4.append(newpoint);
        }
        map.append(v);location.append(v);stench.append(v);breeze.append(v);doubtmonster.append(v);doubtpit.append(v);visited.append(v2);safety.append(v3);room.append(v4);
    }
    location[0][0]=1;//1为智能体，2为怪物，3为陷阱，4为金子
}

void Widget::timeout1(){//计时器槽函数，更新智能体的动作，更新页面
    if(gameover){
        return;
    }
    if(back){
        goback();
    }
    else{
        onlineDFS();
    }
    return;
}

void Widget::paintEvent(QPaintEvent *){//绘画事件
    QPainter painter(this);
    QPen blackpen(Qt::black);
    blackpen.setWidth(5);
    QBrush brush(Qt::white);
    QFont font;
    font.setFamily("SimHei");
    font.setPointSize(16);
    font.setBold(true);
    painter.setPen(blackpen);
    painter.setBrush(brush);
    painter.setFont(font);
    for(int i=0;i<4;i++){//画地图
        int y=40+150*i;
        for(int j=0;j<4;j++){
            int x=40+150*j;
            QRect rect(x,y,150,150);
            painter.drawRect(rect);
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(location[i][j]){
                if(location[i][j]==2&&!shootwumpus){
                    painter.drawPixmap(45+150*i,45+150*j,140,140,QPixmap(":/monster.jpg"));//画怪物
                }
                else if(location[i][j]==3){
                    painter.drawPixmap(45+150*i,45+150*j,140,140,QPixmap(":/pit.png"));//画陷阱
                }
                else if(location[i][j]==4&&!findgold){
                    painter.drawPixmap(45+150*i,45+150*j,140,140,QPixmap(":/gold.jpeg"));//画金子
                }
            }
        }
    }
    painter.drawPixmap(45+150*agentx,45+150*agenty,140,140,QPixmap(":/eve.png"));//画智能体
    painter.drawText(890,120,QString::number(score));
    if(gameover){
        painter.drawText(800,170,"Game Over!");
    }
    update();
}

void Widget::goback(){//寻找到出口的最短路径
    score--;
    if(agentx==0&&agenty==0){
        gameover=true;
        score++;
        return;
    }
    if(agenty>0&&visited[agentx][agenty-1]){
        agenty--;
        return;
    }
    else if(agenty>0&&!doubtpit[agentx][agenty-1]&&!doubtmonster[agentx][agenty-1]){
        agenty--;
        return;
    }
    if(agentx>0&&visited[agentx-1][agenty]){
        agentx--;
        return;
    }
    else if(agentx>0&&!doubtpit[agentx-1][agenty]&&!doubtmonster[agentx-1][agenty]){
        agentx--;
        return;
    }
    if(agenty==0){
        agentx++;
        return;
    }
    if(agentx==0){
        agenty++;
        return;
    }
    return;
}

void Widget::onlineDFS(){//联机深度优先搜索
    score--;
    visited[agentx][agenty]=true;
    safety[agentx][agenty]=true;
    point* newpoint=room[agentx][agenty];
    if(agentx==0){
        newpoint->up=true;
    }
    if(agentx==3){
        newpoint->down=true;
    }
    if(agenty==0){
        newpoint->left=true;
    }
    if(agenty==3){
        newpoint->right=true;
    }
    track.append(newpoint);
    if(agentx==goldx&&agenty==goldy){
        back=true;
        findgold=true;
        score+=1000;
        return;
    }
    if(location[agentx][agenty]==2||location[agentx][agenty]==3){
        score-=1000;
        gameover=true;
        return;
    }
    int doubtpitrooms=0;
    int indexpitx=0,indexpity=0;
    if(breeze[agentx][agenty]){//如果当前房间有微风，进行安全性判断
        if(agenty>0&&!visited[agentx][agenty-1]&&safety[agentx][agenty-1]){
            doubtpit[agentx][agenty-1]++;
            doubtpitrooms++;
            indexpitx=agentx;
            indexpity=agenty-1;
            if(doubtpit[agentx][agenty-1]==2){//若一个房间被两次标记为怀疑为有陷阱的房间，则该房间确有陷阱
                safety[agentx][agenty-1]=false;
            }
        }
        if(agenty<3&&!visited[agentx][agenty+1]&&safety[agentx][agenty+1]){
            doubtpit[agentx][agenty+1]++;
            doubtpitrooms++;
            indexpitx=agentx;
            indexpity=agenty+1;
            if(doubtpit[agentx][agenty+1]==2){
                safety[agentx][agenty+1]=false;
            }
        }
        if(agentx>0&&!visited[agentx-1][agenty]&&safety[agentx-1][agenty]){
            doubtpit[agentx-1][agenty]++;
            doubtpitrooms++;
            indexpitx=agentx-1;
            indexpity=agenty;
            if(doubtpit[agentx-1][agenty]==2){
                safety[agentx-1][agenty]=false;
            }
        }
        if(agentx<3&&!visited[agentx+1][agenty]&&safety[agentx+1][agenty]){
            doubtpit[agentx+1][agenty]++;
            doubtpitrooms++;
            indexpitx=agentx+1;
            indexpity=agenty;
            if(doubtpit[agentx+1][agenty]==2){
                safety[agentx+1][agenty]=false;
            }
        }
    }
    else{
        if(agenty>0){
            doubtpit[agentx][agenty-1]=0;
        }
        if(agenty<3){
            doubtpit[agentx][agenty+1]=0;
        }
        if(agentx>0){
            doubtpit[agentx-1][agenty]=0;
        }
        if(agentx<3){
            doubtpit[agentx+1][agenty]=0;
        }
    }
    if(doubtpitrooms==1){
        safety[indexpitx][indexpity]=false;//可将某一房间排除为有陷阱的房间，为不安全的房间
    }
    int doubtmonsterrooms=0;
    int indexmonsterx=0,indexmonstery=0;
    if(stench[agentx][agenty]&&!findwumpus){//如果当前房间有恶臭，进行安全性判断，并准备射箭
        if(agenty>0&&!visited[agentx][agenty-1]&&safety[agentx][agenty-1]&&!shootwumpus){
            doubtmonsterrooms++;
            indexmonsterx=agentx;
            indexmonstery=agenty-1;
            if(arrow){//有恶臭时若有箭就射箭判断怪物位置以确保安全
                score-=10;
                if(location[agentx][agenty-1]==2){//射中了就可以确定世界上没有怪物的威胁了
                    shootwumpus=true;
                    if(doubtpit[agentx][agenty-1]){
                        doubtpit[agentx][agenty-1]=0;
                    }
                    clearstench();
                }
                else{//否则至少可以确定射箭的这个房间是安全的
                    safety[agentx][agenty-1]=true;
                    doubtmonsterrooms--;
                }
                arrow--;
            }
            else{//若无箭则只能将这个房间标记为怀疑有怪物
                doubtmonster[agentx][agenty-1]++;
                doubtmonsterrooms++;
                if(doubtmonster[agentx][agenty-1]==2){//若一个房间被两次标记为怀疑为有Wumpus的房间，则该房间确有Wumpus
                    safety[agentx][agenty-1]=false;
                    findwumpus=true;
                    wumpusx=agentx;
                    wumpusy=agenty-1;
                    cancelmonsterdoubt();
                }
            }
        }
        if(agenty<3&&!visited[agentx][agenty+1]&&safety[agentx][agenty+1]&&!shootwumpus){
            doubtmonsterrooms++;
            indexmonsterx=agentx;
            indexmonstery=agenty+1;
            if(arrow){//有恶臭时就射箭判断怪物位置以确保安全
                score-=10;
                if(location[agentx][agenty+1]==2){//射中了就可以确定世界上没有怪物的威胁了
                    shootwumpus=true;
                    if(doubtpit[agentx][agenty+1]){
                        doubtpit[agentx][agenty+1]=0;
                    }
                    clearstench();
                }
                else{
                    safety[agentx][agenty+1]=true;
                    doubtmonsterrooms--;
                }
                arrow--;
            }
            else{
                doubtmonster[agentx][agenty+1]++;
                doubtmonsterrooms++;
                if(doubtmonster[agentx][agenty+1]==2){
                    safety[agentx][agenty+1]=false;
                    findwumpus=true;
                    wumpusx=agentx;
                    wumpusy=agenty+1;
                    cancelmonsterdoubt();
                }
            }
        }
        if(agentx>0&&!visited[agentx-1][agenty]&&safety[agentx-1][agenty]&&!shootwumpus){
            doubtmonsterrooms++;
            indexmonsterx=agentx-1;
            indexmonstery=agenty;
            if(arrow){//有恶臭时就射箭判断怪物位置以确保安全
                score-=10;
                if(location[agentx-1][agenty]==2){
                    shootwumpus=true;
                    if(doubtpit[agentx-1][agenty]){
                        doubtpit[agentx-1][agenty]=0;
                    }
                    clearstench();
                }
                else{
                    safety[agentx-1][agenty]=true;
                    doubtmonsterrooms--;
                }
                arrow--;
            }
            else{
                doubtmonster[agentx-1][agenty]++;
                doubtmonsterrooms++;
                if(doubtmonster[agentx-1][agenty]==2){
                    safety[agentx-1][agenty]=false;
                    findwumpus=true;
                    wumpusx=agentx-1;
                    wumpusy=agenty;
                    cancelmonsterdoubt();
                }
            }
        }
        if(agentx<3&&!visited[agentx+1][agenty]&&safety[agentx+1][agenty]&&!shootwumpus){
            doubtmonsterrooms++;
            indexmonsterx=agentx+1;
            indexmonstery=agenty;
            if(arrow){//有恶臭时就射箭判断怪物位置以确保安全
                score-=10;
                if(location[agentx+1][agenty]==2){
                    shootwumpus=true;
                    if(doubtpit[agentx+1][agenty]){
                        doubtpit[agentx+1][agenty]=0;
                    }
                    clearstench();
                }
                else{
                    safety[agentx+1][agenty]=true;
                    doubtmonsterrooms--;
                }
                arrow--;
            }
            else{
                doubtmonster[agentx+1][agenty]++;
                doubtmonsterrooms++;
                if(doubtmonster[agentx+1][agenty]==2){
                    safety[agentx+1][agenty]=false;
                    findwumpus=true;
                    wumpusx=agentx+1;
                    wumpusy=agenty;
                    cancelmonsterdoubt();
                }
            }
        }
    }
    else{
        if(agenty>0){
            doubtmonster[agentx][agenty-1]=0;
        }
        if(agenty<3){
            doubtmonster[agentx][agenty+1]=0;
        }
        if(agentx>0){
            doubtmonster[agentx-1][agenty]=0;
        }
        if(agentx<3){
            doubtmonster[agentx+1][agenty]=0;
        }
    }
    if(doubtmonsterrooms==1){
        safety[indexmonsterx][indexmonstery]=false;//可将某一房间排除为有陷阱的房间，为不安全的房间
        doubtmonster[indexmonsterx][indexmonstery]=2;
        cancelmonsterdoubt();
    }
    int posx=agentx,posy=agenty;
    QString direction=" ";
    if(agenty<3&&!newpoint->right){               /*开始寻找下一步应该往哪个方向走*/
        if(visited[agentx][agenty+1]){//如果房间被走过了说明是安全的可以走，但不是最佳选择，作为备选
            posx=agentx;
            posy=agenty+1;
            direction="right";
        }
        else{
            if(!doubtmonster[agentx][agenty+1]&&!doubtpit[agentx][agenty+1]){//如果房间既没有被怀疑为有怪物，也没有被怀疑有陷阱，且未被走过，则优先选择进行探索
                agenty++;
                newpoint->right=true;
                return;
            }
        }
    }
    if(agentx<3&&!newpoint->down){
        if(visited[agentx+1][agenty]){
            posx=agentx+1;
            posy=agenty;
            direction="down";
        }
        else{
            if(!doubtmonster[agentx+1][agenty]&&!doubtpit[agentx+1][agenty]){
                agentx++;
                newpoint->down=true;
                return;
            }
        }
    }
    if(agenty>0&&!newpoint->left){
        if(visited[agentx][agenty-1]){
            posx=agentx;
            posy=agenty-1;
            direction="left";
        }
        else{
            if(!doubtmonster[agentx][agenty-1]&&!doubtpit[agentx][agenty-1]){
                agenty--;
                newpoint->left=true;
                return;
            }
        }
    }
    if(agentx>0&&!newpoint->up){
        if(visited[agentx-1][agenty]){
            posx=agentx-1;
            posy=agenty;
            direction="up";
        }
        else{
            if(!doubtmonster[agentx-1][agenty]&&!doubtpit[agentx-1][agenty]){
                agentx--;
                newpoint->up=true;
                return;
            }
        }
    }
    agentx=posx;        /*如果没能找出安全的未走过的房间，则重回老路，确保安全性*/
    agenty=posy;
    if(direction=="right"){
        newpoint->right=true;
    }
    else if(direction=="left"){
        newpoint->left=true;
    }
    else if(direction=="up"){
        newpoint->up=true;
    }
    else if(direction=="down"){
        newpoint->down=true;
    }
    else{
        back=true;
        score++;
    }
    return;
}

void Widget::clearstench(){//清空臭味
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            stench[i][j]=0;
            if(location[i][j]==2){
                location[i][j]=0;
            }
        }
    }
    cancelmonsterdoubt();
}

void Widget::cancelmonsterdoubt(){//将怀疑为有怪物的房间解除怀疑
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(doubtmonster[i][j]==1){
                doubtmonster[i][j]=0;
            }
        }
    }
}

void Widget::createmonster(){//初始化怪物
    int a=qrand()%4;
    int b=qrand()%4;
    while(location[a][b]){
        a=qrand()%4;
        b=qrand()%4;
    }
    location[a][b]=2;
    if(a>0){
        stench[a-1][b]=1;
    }
    if(a<3){
        stench[a+1][b]=1;
    }
    if(b>0){
        stench[a][b-1]=1;
    }
    if(b<3){
        stench[a][b+1]=1;
    }
    return;
}

void Widget::createpit(){//初始化陷阱
    int a=qrand()%4;
    int b=qrand()%4;
    while(location[a][b]){
        a=qrand()%4;
        b=qrand()%4;
    }
    location[a][b]=3;
    if(a>0){
        breeze[a-1][b]=1;
    }
    if(a<3){
        breeze[a+1][b]=1;
    }
    if(b>0){
        breeze[a][b-1]=1;
    }
    if(b<3){
        breeze[a][b+1]=1;
    }
//    for(int a=0;a<4;a++){
//        for(int b=0;b<4;b++){
//            if(!location[a][b]){
//                int probability=qrand()%100;
//                if(probability<=20){
//                    location[a][b]=3;
//                    if(a>0){
//                        breeze[a-1][b]=1;
//                    }
//                    if(a<3){
//                        breeze[a+1][b]=1;
//                    }
//                    if(b>0){
//                        breeze[a][b-1]=1;
//                    }
//                    if(b<3){
//                        breeze[a][b+1]=1;
//                    }
//                }
//            }
//        }
//    }
    return;
}

void Widget::creategold(){//初始化金子
    int a=qrand()%4;
    int b=qrand()%4;
    while(location[a][b]){
        a=qrand()%4;
        b=qrand()%4;
    }
    location[a][b]=4;
    goldx=a;goldy=b;
    return;
}

void Widget::on_pushButton_clicked(){//点击start按钮
    createmonster();
    for(int i=0;i<3;i++){
        createpit();
    }
    creategold();
    //createpit();
    //timer->start(1000);
}

void Widget::on_pushButton_2_clicked(){//点击move按钮
    if(gameover){
        return;
    }
    if(back){
        goback();
    }
    else{
        onlineDFS();
    }
    return;
}

void Widget::on_pushButton_3_clicked(){//点击reset按钮
    initial();
}
