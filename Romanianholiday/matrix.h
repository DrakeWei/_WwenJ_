#ifndef MATRIX_H
#define MATRIX_H
#include <QString>
#include <QList>
#include <QQueue>

struct node{
    QString name;
    int pos=0;
    int x=0,y=0;
    int dist=0;
    int f=INT_MAX;
    int g=INT_MAX;
    int parent=0;
};

class Matrix
{
public:
    Matrix();
    ~Matrix(){delete [] VerticesList; delete [] EdgeDistance; }
    QString *VerticesList; 			//顶点表
    QList<node> List;     //存放结点在图中坐标的容器
    int **EdgeDistance;				//城市距离邻接矩阵
    int *f;
    int *g;
    int *h;
    int getVertexPos (QString vertex);  //给出顶点vertex在图中的位置
    void createEdgeDistance();     //构造邻接矩阵]
    void getvalue();
    const int CitySize=20;  //城市个数
    const int maxdistance=1000;   //设置两个不相邻的城市间距
    QString getName (int i);  			//取顶点 i 的值, i 不合理返回0
    int getDistance (int v1, int v2); 		//取边(v1,v2)上权值
    int getFirstNeighbor (int v);       		//取顶点 v 的第一个邻接顶点
    int getNextNeighbor (int v, int w);	     	//取 v 的邻接顶点 w 的下一邻接顶点

    void UCS(int start,int end);
    void greedysearch(int start,int end);
    void A_star(int start,int end);

    void enq(QList<node>& priority,node newnode);
    void Enq(QList<node>& priority,node newnode);

    QList<node> priority;
    QList<node> show;
    QList<node> consider;
    QList<node> considerA;
    int totdistance=0;
    int* path;
    bool* visited;
    bool* included;
    int exploreA=1;
    int exploreB=1;
    int exploreC=1;
};

#endif // MATRIX_H
