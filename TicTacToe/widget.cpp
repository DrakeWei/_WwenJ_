#include "widget.h"
#include "ui_widget.h"
#include <QFont>
#include <Windows.h>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("井字棋游戏");
    QFont font;
    font.setFamily("SimHei");
    font.setPointSize(30);
    for(int i=0;i<3;i++){
        QVector<int>v;
        for(int j=0;j<3;j++){
            v.append(0);
        }
        chessboard.append(v);
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *){
    QPainter painter(this);
    QFont font;
    font.setFamily("SimHei");
    font.setPointSize(30);
    painter.setFont(font);
    if(manwin){
        painter.drawText(640,300,"玩家获胜！");
    }
    if(machinewin){
        painter.drawText(640,300,"Agent获胜！");
    }
    if(tie){
        painter.drawText(640,300,"平局！");
    }
    update();
}

int Widget::Minimax(QVector<QVector<int>> board, int layer, QString player, int a, int b,bool first){//极小化极大算法，采用Alpha_Beta剪枝
    int score=evaluate(board,layer);
    if(score||!layer){
        return score;
    }
    if(player=="machine"){

        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (!board[i][j])
                {
                    // Make the move
                    if(choosex){
                     board[i][j]=2;
                    }
                    else{
                        board[i][j]=1;
                    }

                    // Call minimax recursively and choose
                    // the maximum value
                    int score = Minimax(board, layer-1, "man",a,b,false);
                    // Undo the move
                    board[i][j] = 0;

                    if(score>a){
                        a=score;
                        if(first){
                            decidex=i;
                            decidey=j;
                        }
                    }
                    if(a>=b){
                        break;
                    }
                }
            }
        }
        return a;
    }
    else
    {

        // Traverse all cells
        for (int i = 0; i<3; i++)
        {
            for (int j = 0; j<3; j++)
            {
                // Check if cell is empty
                if (!board[i][j])
                {
                    // Make the move
                    if(choosex){
                        board[i][j]=1;
                    }
                    else{
                        board[i][j]=2;
                    }

                    // Call minimax recursively and choose
                    // the minimum value
                    int score = Minimax(board, layer-1, "machine",a,b,false);

                    // Undo the move
                    board[i][j] = 0;

                    if(score<b){
                        b=score;
                    }

                    if(a>=b){
                        break;
                    }
                }
            }
        }
        return b;
    }
}

int Widget::evaluate(QVector<QVector<int>>board, int layer){//真正的评估函数！
    for(int i=0;i<3;i++){
        if(board[i][0]==board[i][1]&&board[i][1]==board[i][2]){
            if(board[i][0]==machinechoose){
                return 10-(9-layer);
            }
            else if(board[i][0]==manchoose){
                return -10+(9-layer);
            }
        }
        if(board[0][i]==board[1][i]&&board[1][i]==board[2][i]){
            if(board[0][i]==machinechoose){
                return 10-(9-layer);
            }
            else if(board[0][i]==manchoose){
                return -10+(9-layer);
            }
        }
    }
    if(board[0][0]==board[1][1]&&board[1][1]==board[2][2]){
        if(board[0][0]==machinechoose){
            return 10-(9-layer);
        }
        else if(board[0][0]==manchoose){
            return -10+(9-layer);
        }
    }
    if(board[2][0]==board[1][1]&&board[1][1]==board[0][2]){
        if(board[2][0]==machinechoose){
            return 10-(9-layer);
        }
        else if(board[2][0]==manchoose){
            return -10+(9-layer);
        }
    }
    return 0;
}

void Widget::drawdecision(){//展示决策
    chessleft--;
    if(decidex==0&&decidey==0){
        ui->pushButton_1->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_1->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_1->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==0&&decidey==1){
        ui->pushButton_2->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_2->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_2->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==0&&decidey==2){
        ui->pushButton_3->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_3->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_3->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==1&&decidey==0){
        ui->pushButton_4->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_4->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_4->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==1&&decidey==1){
        ui->pushButton_5->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_5->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_5->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==1&&decidey==2){
        ui->pushButton_6->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_6->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_6->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==2&&decidey==0){
        ui->pushButton_7->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_7->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_7->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==2&&decidey==1){
        ui->pushButton_8->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_8->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_8->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
    else if(decidex==2&&decidey==2){
        ui->pushButton_9->setStyleSheet("QPushButton{font:80px;}");
        if(choosex){
            ui->pushButton_9->setText("○");
            chessboard[decidex][decidey]=2;
        }
        else{
            ui->pushButton_9->setText("×");
            chessboard[decidex][decidey]=1;
        }
    }
}

int Widget::judge(){//棋盘下满时判定胜负或平局
    for(int i=0;i<3;i++){
        int xhorizon=0,xvertic=0,ohorizon=0,overtic=0;
        for(int j=0;j<3;j++){
            if(chessboard[i][j]==1){
                xhorizon++;
            }
            if(chessboard[j][i]==1){
                xvertic++;
            }
            if(chessboard[i][j]==2){
                ohorizon++;
            }
            if(chessboard[j][i]==2){
                overtic++;
            }
        }
        if(xhorizon==3||xvertic==3){
            return 1;
        }
        if(ohorizon==3||overtic==3){
            return 2;
        }
    }
    if(chessboard[0][0]==1&&chessboard[1][1]==1&&chessboard[2][2]==1){
        return 1;
    }
    if(chessboard[0][2]==1&&chessboard[1][1]==1&&chessboard[2][0]==1){
        return 1;
    }
    if(chessboard[0][0]==2&&chessboard[1][1]==2&&chessboard[2][2]==2){
        return 2;
    }
    if(chessboard[0][2]==2&&chessboard[1][1]==2&&chessboard[2][0]==2){
        return 2;
    }
    return 0;
}

void Widget::on_pushButton_10_clicked(){//玩家选择以“×”先手开局
    choosex=true;
    manchoose=1;
    machinechoose=2;
}

void Widget::on_pushButton_11_clicked(){//玩家选择以“○”后手开局
    chooseo=true;
    manchoose=2;
    machinechoose=1;
    Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
    drawdecision();
}

void Widget::on_pushButton_12_clicked(){//重新开始
    chooseo=false;
    choosex=false;
    manchoose=0;
    machinechoose=0;
    manwin=false;
    machinewin=false;
    tie=false;
    chessleft=9;
    ui->pushButton_1->setText(" ");
    ui->pushButton_2->setText(" ");
    ui->pushButton_3->setText(" ");
    ui->pushButton_4->setText(" ");
    ui->pushButton_5->setText(" ");
    ui->pushButton_6->setText(" ");
    ui->pushButton_7->setText(" ");
    ui->pushButton_8->setText(" ");
    ui->pushButton_9->setText(" ");
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            chessboard[i][j]=0;
        }
    }
}

void Widget::on_pushButton_1_clicked(){//点击棋盘
    ui->pushButton_1->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_1->setText("×");
        chessboard[0][0]=1;
    }
    else{
        ui->pushButton_1->setText("○");
        chessboard[0][0]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_2_clicked(){
    ui->pushButton_2->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_2->setText("×");
        chessboard[0][1]=1;
    }
    else{
        ui->pushButton_2->setText("○");
        chessboard[0][1]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_3_clicked(){
    ui->pushButton_3->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_3->setText("×");
        chessboard[0][2]=1;
    }
    else{
        ui->pushButton_3->setText("○");
        chessboard[0][2]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_4_clicked(){
    ui->pushButton_4->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_4->setText("×");
        chessboard[1][0]=1;
    }
    else{
        ui->pushButton_4->setText("○");
        chessboard[1][0]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_5_clicked(){
    ui->pushButton_5->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_5->setText("×");
        chessboard[1][1]=1;
    }
    else{
        ui->pushButton_5->setText("○");
        chessboard[1][1]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_6_clicked(){
    ui->pushButton_6->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_6->setText("×");
        chessboard[1][2]=1;
    }
    else{
        ui->pushButton_6->setText("○");
        chessboard[1][2]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_7_clicked(){
    ui->pushButton_7->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_7->setText("×");
        chessboard[2][0]=1;
    }
    else{
        ui->pushButton_7->setText("○");
        chessboard[2][0]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_8_clicked(){
    ui->pushButton_8->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_8->setText("×");
        chessboard[2][1]=1;
    }
    else{
        ui->pushButton_8->setText("○");
        chessboard[2][1]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}

void Widget::on_pushButton_9_clicked(){
    ui->pushButton_9->setStyleSheet("QPushButton{font:80px;}");
    if(choosex){
        ui->pushButton_9->setText("×");
        chessboard[2][2]=1;
    }
    else{
        ui->pushButton_9->setText("○");
        chessboard[2][2]=2;
    }
    chessleft--;
    if(chessleft){
        Minimax(chessboard,chessleft,"machine",-INT_MAX,INT_MAX,true);
        drawdecision();
    }
    int judgement=judge();
    if(judgement==manchoose){
        manwin=true;
    }
    else if(judgement==machinechoose){
        machinewin=true;
    }
    else if(judgement==0&&!chessleft){
        tie=true;
    }
}
