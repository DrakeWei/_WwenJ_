#ifndef GRAPH_H
#define GRAPH_H

#include <QString>

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

class Graph
{
public:
    Graph();
};

#endif // GRAPH_H
