// MST-普里姆算法—代码实现
// 采用的是邻接矩阵的方式存储图
// 参考 https://blog.csdn.net/qq_35644234/article/details/59106779

/*
算法思路：
首先就是从图中的一个起点a开始，把a加入U集合，然后，寻找从与a有关联的边中，
权重最小的那条边并且该边的终点b在顶点集合：（V-U）中，我们也把b加入到集合U中，
并且输出边（a，b）的信息，这样我们的集合U就有：{a,b}，
然后，我们寻找与a关联和b关联的边中，权重最小的那条边并且该边的终点在集合：
（V-U）中，我们把c加入到集合U中，并且输出对应的那条边的信息，
这样我们的集合U就有：{a,b,c}这三个元素了，一次类推，直到所有顶点都加入到了集合U。

时间复杂度的分析：
其中我们建立邻接矩阵需要的时间复杂度为：O(n*n),然后，
我们Prim函数中生成最小生成树的时间复杂度为：O(n*n).

输入：

6 10
1 2 6
1 3 1
1 4 5
2 3 5
2 5 3
3 5 6
3 6 4
4 3 5
4 6 2
5 6 6
1
2
3
4
5
6
7
8
9
10
11
12
输出：


*/

#include <iostream>
#include <string>
#include <vector>
using  namespace std;

//首先是使用邻接矩阵完成Prim算法
struct Graph {
    int vexnum;  //顶点个数
    int edge;   //边的条数
    int ** arc; //邻接矩阵
    string *information; //记录每个顶点名称
};

// dev 不认识 to_string()
// https://blog.csdn.net/xiao_song_shu/article/details/76933854

// dev c++ 不认识的话
// #define INT_MAX 10000

//创建图
void createGraph(Graph & g) {
    cout << "请输入顶点数：输入边的条数" << endl;
    cin >> g.vexnum;
    cin >> g.edge;  //输入边的条数

    g.information = new string[g.vexnum];
    g.arc = new int*[g.vexnum];
    int i = 0;

    //开辟空间的同时，进行名称的初始化
    for (i = 0; i < g.vexnum; i++) {
        g.arc[i] = new int[g.vexnum];
        g.information[i]="v"+ std::to_string(i+1);//对每个顶点进行命名
        for (int k = 0; k < g.vexnum; k++) {
            g.arc[i][k] = INT_MAX;          //初始化我们的邻接矩阵
        }
    }

    cout << "请输入每条边之间的顶点编号(顶点编号从1开始),以及该边的权重：" << endl;
    for (i = 0; i < g.edge; i++) {
        int start;
        int end;
        cin >> start;   //输入每条边的起点
        cin >> end;     //输入每条边的终点
        int weight;
        cin >> weight;
        g.arc[start-1][end-1]=weight;//无向图的边是相反的
        g.arc[end-1][start-1] = weight;
    }
}

//打印图
void print(Graph g) {
    int i;
    for (i = 0; i < g.vexnum; i++) {
        //cout << g.information[i] << " ";
        for (int j = 0; j < g.vexnum; j++) {
            if (g.arc[i][j] == INT_MAX)
                cout << "∞" << " ";
            else
            cout << g.arc[i][j] << " ";
        }
        cout << endl;
    }
}

//作为记录边的信息，这些边都是达到end的所有边中，权重最小的那个
struct Assis_array {
    int start; //边的终点
    int end;  //边的起点
    int weight;  //边的权重
};
//进行prim算法实现，使用的邻接矩阵的方法实现。
void Prim(Graph g,int begin) {

    //close_edge这个数组记录到达某个顶点的各个边中的权重最大的那个边
    Assis_array *close_edge=new Assis_array[g.vexnum];

    int j;

    //进行close_edge的初始化，更加开始起点进行初始化
    for (j = 0; j < g.vexnum; j++) {
        if (j != begin - 1) {
            close_edge[j].start = begin-1;
            close_edge[j].end = j;
            close_edge[j].weight = g.arc[begin - 1][j];
        }
    }
    //把起点的close_edge中的值设置为-1，代表已经加入到集合U了
    close_edge[begin - 1].weight = -1;
    //访问剩下的顶点，并加入依次加入到集合U
    for (j = 1; j < g.vexnum; j++) {

        int min = INT_MAX;
        int k;
        int index;
        //寻找数组close_edge中权重最小的那个边
        for (k = 0; k < g.vexnum; k++) {
            if (close_edge[k].weight != -1) {
                if (close_edge[k].weight < min) {
                    min = close_edge[k].weight;
                    index = k;
                }
            }
        }
        //将权重最小的那条边的终点也加入到集合U
        close_edge[index].weight = -1;
        //输出对应的边的信息
        cout << g.information[close_edge[index].start]
            << "-----"
            << g.information[close_edge[index].end]
            << "="
            <<g.arc[close_edge[index].start][close_edge[index].end]
            <<endl;

        //更新我们的close_edge数组。
        for (k = 0; k < g.vexnum; k++) {
            if (g.arc[close_edge[index].end][k] <close_edge[k].weight) {
                close_edge[k].weight = g.arc[close_edge[index].end][k];
                close_edge[k].start = close_edge[index].end;
                close_edge[k].end = k;
            }
        }
    }
}



int main()
{
    Graph g;
    createGraph(g);//基本都是无向网图，所以我们只实现了无向网图
    print(g);
    Prim(g, 1);
    system("pause");
    return 0;
}
