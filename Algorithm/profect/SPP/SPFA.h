/************************************************************/
/*                程序作者：Willam                          */
/*                程序完成时间：2017/3/12                   */
/*                有任何问题请联系：2930526477@qq.com       */
/************************************************************/
//@尽量写出完美的程序


//#pragma once是一个比较常用的C/C++杂注，
//只要在头文件的最开始加入这条杂注，
//就能够保证头文件只被编译一次。
#pragma once

#include<iostream>
#include<string>
#include<queue>
using namespace std;
/*
本算法是使用SPFA来求解图的单源最短路径问题
采用了邻接表作为图的存储结构
可以应用于任何无环的图
*/

//表结构
struct ArcNode {
    int adjvex;      //边的另外一边的顶点下标
    ArcNode * next; //下一条边的表结点
    int weight;
};
struct Vnode {
    string data;           //顶点信息
    ArcNode * firstarc;  //第一条依附在该顶点的边
};

struct Dis {
    string path;  //从顶点到该该顶点最短路径
    int  weight;  //最短路径的权重
};

class Graph {
private:
    int vexnum;    //边的个数
    int edge;       //边的条数
    Vnode * node; //邻接表
    Dis  * dis;   //记录最短路径信息的数组
public:
    Graph(int vexnum, int edge);
    ~Graph();
    void createGraph(int); //创建图
    bool check_edge_value(int , int ,int); //判断边的信息是否合法
    void print();  //打印图的邻接表
    bool SPFA(int begin);   //求解最短路径
    void print_path(int begin); //打印最短路径
};
