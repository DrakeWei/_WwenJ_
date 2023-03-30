#include "graphlnk.h"

Graphlnk::Graphlnk()//邻接表构造函数
{
    NodeTable=new Vertex[8];
    numVertices=0;
    for (int i = 0; i < 8; i++){
    NodeTable[i].adj = NULL;
    NodeTable[i].data=' ';
    visited=new bool[numVertices];}
}

bool Graphlnk::insertVertex(const QString& vertex){//图中插入一个顶点
    NodeTable[numVertices++].data = vertex;	//插入在表的最后
    return true;
}

bool Graphlnk::insertEdge(int v1, int v2, int weight)//图中插入一条边
{     if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices)
    {   	Edge *q, *p = NodeTable[v1].adj;   	//v1对应的边链表头指针
        while (p != NULL && p->dest != v2)  p = p->link;   //寻找邻接顶点v2
        if (p != NULL) 	return false;     		//若此条边已存在,则不插入
        p = new Edge;			//否则, 创建新结点,插在一条链表中的前端
        q = new Edge;
        p->dest = v2;  p->cost = weight;
        p->link = NodeTable[v1].adj; 		//链入v1边链表
        NodeTable[v1].adj = p;
        q->dest = v1;  q->cost = weight;
        q->link = NodeTable[v2].adj; 		//链入v2边链表
        NodeTable[v2].adj = q;
        numEdges++;
        return true;
    }
    return false;
}

QString Graphlnk::getValue(int i){
    return NodeTable[i].data;
}

int Graphlnk::getFirstNeighbor (int v) {
//给出顶点位置为 v 的第一个邻接顶点的位置, 如果找不到, 则函数返回-1
    if (v != -1)
    {			//顶点v存在
        Edge *p = NodeTable[v].adj;		//对应边链表第一个边结点
        if (p != NULL)  return p->dest;		//存在, 返回第一个邻接顶点
   }
   return -1;		//第一个邻接顶点不存在
};

int Graphlnk::getNextNeighbor (int v, int w) {
//给出顶点v的邻接顶点w的下一个邻接顶点的位置,若没有下一个邻接顶点, 则函数返回-1
    if (v != -1) {				//顶点v存在
            Edge *p = NodeTable[v].adj;
            while (p != NULL && p->dest != w)
                p = p->link;
            if (p != NULL && p->link != NULL)
                return p->link->dest; 	//返回下一个邻接顶点
    }
    return -1; 			//下一邻接顶点不存在
};

int Graphlnk::getWeight(int v1, int v2){//获取两个顶点边的权值（或是判断两点之间是否有边）
    Edge* e=NodeTable[v1].adj;
    while(e){
        if(e->dest==v2){
            return e->cost;
        }
        else{
            e=e->link;
        }
    }
    return 0;
}
