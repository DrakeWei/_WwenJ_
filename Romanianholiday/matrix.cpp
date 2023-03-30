#include "matrix.h"
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDir>
#include <QTextCodec>
#include <QDebug>

Matrix::Matrix()
{
    VerticesList=new QString[CitySize];
    EdgeDistance = (int **) new int *[CitySize];	//创建邻接矩阵
    f=new int[CitySize];
    g=new int[CitySize];
    h=new int[CitySize];
    visited=new bool[CitySize];
    included=new bool[CitySize];
    path =new int[CitySize];
    for (int i = 0; i < CitySize; i++)
        EdgeDistance[i] = new int[CitySize];
    for(int i=0;i<CitySize;i++){
        for(int j=0;j<CitySize;j++){
            EdgeDistance[i][j]=0;
        }
        visited[i]=false;
        included[i]=false;
        f[i]=INT_MAX;
        g[i]=INT_MAX;
    }
}

void Matrix::createEdgeDistance(){//构造距离邻接矩阵
        QFile file(":/罗马尼亚度假问题地图.csv");//打开csv文件
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Open Romanianholiday file failed!";
            return;
        }
        QStringList list;
        list.clear();
        QTextStream in(&file);
        //遍历行
        for(int i = 0; !in.atEnd(); i++)
        {
            QString fileLine = in.readLine();
            list = fileLine.split(",", QString::SkipEmptyParts);    //根据","开分隔开每行的列
            if(i==0){
                for(int j=0;j<list.size();j++){
                    QString str=list.at(j);
                    VerticesList[j]=str;
                    node newnode;
                    newnode.name=str;
                    newnode.pos=j;
                    List.append(newnode);
                }
            }
            if(i >= 1)
            {
                for(int j = 1; j < list.size(); j++)
                {
                    QString str=list.at(j);
                    int distance=str.toInt();
                    EdgeDistance[i-1][j-1]=distance;
                }
            }
        }
        file.close();
}

void Matrix::getvalue(){//读取启发函数值
    QFile file(":/罗马尼亚度假问题启发函数值.csv");//打开csv文件
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Open Romanianholiday file failed!";
        return;
    }
    QStringList list;
    list.clear();
    QTextStream in(&file);
    //遍历行
    for(int i = 0; !in.atEnd(); i++)
    {
        if(i==20){
            break;
        }
        QString fileLine = in.readLine();
        list = fileLine.split(",", QString::SkipEmptyParts);    //根据","开分隔开每行的列
        QString str2=list.at(1);
        int function=str2.toInt();
        h[i]=function;
    }
    file.close();
}

QString Matrix::getName(int i){//获取结点名称
    return VerticesList[i];
}

int Matrix::getDistance(int v1, int v2){//获取两个结点之间的实际距离
    return EdgeDistance[v1][v2];
}

int Matrix::getFirstNeighbor(int v){//获取一个结点的第一个邻接结点
    for(int i=0;i<CitySize;i++){
        if(EdgeDistance[v][i]<maxdistance){
            return i;
        }
    }
    return -1;
}

int Matrix::getNextNeighbor(int v, int w){//获取一个结点的下一个邻接结点
    if(w>=CitySize){
        return -1;
    }
    for(int i=w+1;i<CitySize;i++){
        if(EdgeDistance[v][i]<maxdistance){
            return i;
        }
    }
    return -1;
}

int Matrix::getVertexPos(QString vertex){//获取城市的下标
    for (int i = 0; i < CitySize; i++)
        if (VerticesList[i] == vertex) return i;
    return -1;
}

void Matrix::enq(QList<node> &priority, node newnode){//自建的优先级队列(1)    供一致搜索算法使用
    if(priority.empty()){
        priority.append(newnode);
        return;
    }
    priority.append(newnode);
    int index=priority.size()-1;
    for(int i=index-1;i>=0;i--){
        if(newnode.dist<priority[i].dist){
            priority.swap(i,index);
            index=i;
        }
        else{
            break;
        }
    }
    return;
}

void Matrix::Enq(QList<node> &priority, node newnode){//自建的优先级队列(2)    供A*算法使用
    if(priority.empty()){
        priority.append(newnode);
        return;
    }
    priority.append(newnode);
    int index=priority.size()-1;
    for(int i=index-1;i>=0;i--){
        if(newnode.f<priority[i].f){
            priority.swap(i,index);
            index=i;
        }
        else{
            break;
        }
    }
    return;
}

void Matrix::UCS(int start, int end){//一致代价搜索UCS算法
    node usednode;
    while(!priority.empty()){//当优先级队列中元素不为空时
        usednode=priority.at(0);//弹出首元素
        priority.removeFirst();
        consider.append(usednode);
        int pos=getVertexPos(usednode.name);
        if(pos==end){//若弹出结点为要找的目的地，则循环结束，找到最短路径
            break;
        }
        for(int i=0;i<CitySize;i++){//否则扩展该结点，探寻该结点的所有邻接结点
            if(EdgeDistance[pos][i]>0&&EdgeDistance[pos][i]<maxdistance){
                if(!visited[i]){//若该结点不是目的地但未被探寻过，则将该结点放入优先级队列,否则放弃
                    exploreA++;
                    node newnode;
                    newnode.name=getName(i);
                    newnode.pos=i;
                    newnode.x=List[i].x;
                    newnode.y=List[i].y;
                    newnode.dist=usednode.dist+EdgeDistance[pos][i];//记录其到出发地的实际距离
                    path[i]=pos;
                    visited[i]=true;
                    enq(priority,newnode);
                }
                else{
                    if(i==end){//若该结点已被探寻过但是目的地，则仍将其放入优先级队列中
                        exploreA++;
                        node newnode;
                        newnode.name=getName(i);
                        newnode.pos=i;
                        newnode.x=List[i].x;
                        newnode.y=List[i].y;
                        newnode.dist=usednode.dist+EdgeDistance[pos][i];//记录其到出发地的实际距离
                        path[i]=pos;
                        enq(priority,newnode);
                    }
                }
            }
        }
    }
    totdistance=usednode.dist;
    return;
}

void Matrix::greedysearch(int start, int end){//贪婪算法
    if(start==end){//若搜到的结点为目的地，则结束递归，找到最短路径
        show.append(List[end]);
        return;
    }
    show.append(List[start]);
    int min=INT_MAX;
    int pos=0;
    for(int i=0;i<CitySize;i++){//探索当前结点的所有邻接结点
        if(EdgeDistance[start][i]>0&&EdgeDistance[start][i]<maxdistance){
            exploreB++;
            if(min>h[i]){//在所有邻接结点中找到距离目的地代价最短的结点，并将其作为下一次递归的出发点
                min=h[i];
                pos=i;
            }
        }
    }
    totdistance+=EdgeDistance[start][pos];
    greedysearch(pos,end);
    return;
}

void Matrix::A_star(int start, int end){//A*算法
    node usednode;
    while(!priority.empty()){//若当前优先级队列不为空则弹出首元素
        usednode=priority.at(0);
        priority.removeFirst();
        considerA.append(usednode);
        int pos=getVertexPos(usednode.name);
        if(pos==end){//若当前结点为目的地，则停止循环，找到最短路径
            break;
        }
        int i=0;
        for(;i<CitySize;i++){//探寻该结点的所有邻接结点
            if(EdgeDistance[pos][i]>0&&EdgeDistance[pos][i]<maxdistance){
                exploreC++;
                node newnode;
                newnode.name=getName(i);
                newnode.pos=i;
                newnode.x=List[i].x;
                newnode.y=List[i].y;
                newnode.g=usednode.g+EdgeDistance[pos][i];//计算结点g值
                newnode.f=h[newnode.pos]+newnode.g;//f=h+g  计算结点f值
                newnode.parent=considerA.size()-1;
                Enq(priority,newnode);
            }
        }
    }
    totdistance=usednode.f;
    return;
}
