/************************************************************/
/*                程序作者：Willam                          */
/*                程序完成时间：2017/3/11                   */
/*                有任何问题请联系：2930526477@qq.com       */
/************************************************************/
//@尽量写出完美的程序

#pragma once
//#pragma once是一个比较常用的C/C++杂注，
//只要在头文件的最开始加入这条杂注，
//就能够保证头文件只被编译一次。

/*
本博客开始对Floyd算法的使用邻接矩阵实现的
*/

#include<iostream>
#include<string>
using namespace std;

class Graph_DG {
private:
    int vexnum;   //图的顶点个数
    int edge;     //图的边数
    int **arc;   //邻接矩阵
    int ** dis;   //记录各个顶点最短路径的信息
    int ** path;  //记录各个最短路径的信息
public:
    //构造函数
    Graph_DG(int vexnum, int edge);
    //析构函数
    ~Graph_DG();
    // 判断我们每次输入的的边的信息是否合法
    //顶点从1开始编号
    bool check_edge_value(int start, int end, int weight);
    //创建图
    void createGraph(int);
    //打印邻接矩阵
    void print();
    //求最短路径
    void Floyd();
    //打印最短路径
    void print_path();
};
