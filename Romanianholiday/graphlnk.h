#ifndef GRAPHLNK_H
#define GRAPHLNK_H
#include<QString>
#include<QQueue>
#include<QList>

struct Edge {			   	//边结点的定义
    QString data;                 //边的另一顶点的名字
    int dest;					//边的另一顶点位置
    int cost;					//边上的权值
    Edge *link;		   	//下一条边链指针
    Edge () {}					//构造函数
    Edge (int num, int weight) 	   	//构造函数
        : dest (num), cost (weight), link (NULL) { }
    bool operator != (Edge& R) const
    { return dest != R.dest; }	   	//判边等否
};

struct Vertex {			   //顶点的定义
    QString data;				   //顶点的名字
    Edge *adj;		   //边链表的头指针
    int x;             //结点横坐标
    int y;             //结点纵坐标
};

class Graphlnk
{
public:
    Graphlnk();//构造函数
    ~Graphlnk();//析构函数
    int numEdges;				//当前边数
    int numVertices;		    //当前顶点数
    Vertex* NodeTable;        	//顶点表 (存储各条链表的头结点)
    int* f;
    int* g;
    int* h;
    int getVertexPos (const QString vertx) {      //给出顶点vertex在邻接表中的位置
        for (int i = 0; i < numVertices; i++)
            if (NodeTable[i].data == vertx) return i;
        return -1;
    }
    int NumberOfVertices () { return numVertices; }     	//返回当前顶点数
    QString getValue (int i);                 //取第i条链表的头结点的名字
    int getWeight (int v1, int v2);	     	//取边(v1,v2)权值
    bool insertVertex (const QString& vertex); //图中插入顶点
    bool insertEdge (int v1, int v2, int cost);//图中插入边
    int getFirstNeighbor (int v);//获取某一顶点的第一个邻接点
    int getNextNeighbor (int v, int w);//获取某一顶点的某一邻接点的下一邻接点

    bool* visited;
    QQueue<Vertex> queue;
    QList<Vertex> List;
    QList<Vertex> list;
};

#endif // GRAPHLNK_H
