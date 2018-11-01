/*
参考：https://blog.csdn.net/qq_35644234/article/details/59106779
克鲁斯卡算法
算法思路： 
（1）将图中的所有边都隐藏。 
（2）将边按权值从小到大的顺序选出来，保证添加的过程中不会形成环 
（3）重复上一步直到连接所有顶点，此时就生成了最小生成树。这是一种贪心策略。

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

*/

#include<iostream>
#include<algorithm>
#include<string>
using namespace std;

//检验输入边数和顶点数的值是否有效，可以自己推算为啥：
//顶点数和边数的关系是：((Vexnum*(Vexnum - 1)) / 2) < edge
bool check(int Vexnum,int edge) {
    if (Vexnum <= 0 || edge <= 0 || ((Vexnum*(Vexnum - 1)) / 2) < edge)
        return false;
    return true;
}

//判断我们每次输入的的边的信息是否合法
//顶点从1开始编号
bool check_edge(int Vexnum, int start ,int end, int weight) {
    if (start<1 || end<1 || start>Vexnum || end>Vexnum || weight < 0) {
        return false;
    }
    return true;
}

//边集结构，用于保存每条边的信息
typedef struct edge_tag {
    bool visit; //判断这条边是否加入到了最小生成树中
    int start;   //该边的起点
    int end;   //该边的终点
    int weight; //该边的权重
}Edge;

//创建一个图，但是图是使用边集结构来保存
void createGraph(Edge * &e,int Vexnum, int edge) {
    e = new Edge[edge];//为每条边集开辟空间
    int start = 0;
    int end = 0;
    int weight = 0;

    int i = 0;
    cout << "输入每条边的起点、终点和权重：" << endl;
    while (i != edge)
    {
        cin >> start >> end >> weight;
        while (!check_edge(Vexnum, start, end, weight)) {
            cout << "输入的值不合法，请重新输入每条边的起点、终点和权重：" << endl;
            cin >> start >> end >> weight;
        }
        e[i].start = start;
        e[i].end = end;
        e[i].weight = weight;
        e[i].visit = false; //每条边都还没被初始化
        ++i;

    }
}

//我们需要对边集进行排序，排序是按照每条边的权重，从小到大排序。
int cmp(const void*  first, const void * second) {
    return ((Edge *)first)->weight - ((Edge *)second)->weight;
}

//好了，我们现在需要做的是通过一定的方式来判断
//如果我们把当前的边加入到生成树中是否会有环出现。
//通过我们之前学习树的知识，我们可以知道如果很多棵树就组成一个森林，而且
//如果同一颗树的两个结点在连上一条边，那么就会出现环，
//所以我们就通过这个方式来判断加入了一个新的边后，是否会产生环，
//开始我们让我们的图的每个顶点都是一颗独立的树，通过不断的组合，把这个森林变
//成来源于同一颗顶点的树
//如果不理解，画个图就明白了，

//首先是找根节点的函数,
//其中parent代表顶点所在子树的根结点
//child代表每个顶点孩子结点的个数
int find_root(int child, int * parent) {

    //此时已经找到了该顶点所在树的根节点了
    if (parent[child] == child) {
        return child;
    }
    //往前递归，寻找它父亲的所在子树的根结点
    parent[child] = find_root(parent[child], parent);
    return parent[child];
}

//合并两个子树
bool union_tree(Edge  e, int * parent, int * child) {
    //先找出改边所在子树的根节点
    int root1;
    int root2;
    //记住我们顶点从1开始的，所以要减1
    root1 = find_root(e.start-1, parent);
    root2 = find_root(e.end-1, parent);
    //只有两个顶点不在同一颗子树上，才可以把两棵树并未一颗树
    if (root1 != root2) {
        //小树合并到大树中，看他们的孩子个数
        if (child[root1] > child[root2]) {
            parent[root2] = root1;
            //大树的孩子数量是小树的孩子数量加上
            //大树的孩子数量在加上小树根节点自己
            child[root1] += child[root2] + 1;
        }
        else {
            parent[root1] = root2;
            child[root2] += child[root1] + 1;
        }
        return true;
    }
    return false;
}

//克鲁斯卡算法的实现
void Kruskal() {
    int Vexnum = 0;
    int edge = 0;
    cout << "请输入图的顶点数和边数：" << endl;
    cin >> Vexnum >> edge;
    while (!check(Vexnum, edge)) {
        cout << "你输入的图的顶点数和边数不合法，请重新输入：" << endl;
        cin >> Vexnum >> edge;
    }

    //声明一个边集数组
    Edge * edge_tag;
    //输入每条边的信息
    createGraph(edge_tag, Vexnum, edge);

    int * parent = new int[Vexnum]; //记录每个顶点所在子树的根节点下标
    int * child = new int[Vexnum]; //记录每个顶点为根节点时，其有的孩子节点的个数
    int i;
    for (i = 0; i < Vexnum; i++) {
        parent[i] = i;
        child[i] = 0;
    }
    //对边集数组进行排序，按照权重从小到达排序
    qsort(edge_tag, edge, sizeof(Edge), cmp);
    int count_vex; //记录输出的边的条数

    count_vex = i = 0;
    while (i != edge) {
        //如果两颗树可以组合在一起，说明该边是生成树的一条边
        if (union_tree(edge_tag[i], parent, child)) {
            cout << ("v" + std::to_string(edge_tag[i].start))
                << "-----"
                << ("v" + std::to_string(edge_tag[i].end))
                <<"="
                << edge_tag[i].weight
                << endl;
            edge_tag[i].visit = true;
            ++count_vex; //生成树的边加1
        }
        //这里表示所有的边都已经加入成功
        if (count_vex == Vexnum - 1) {
            break;
        }
        ++i;
    }

    if (count_vex != Vexnum - 1) {
        cout << "此图为非连通图！无法构成最小生成树。" << endl;
    }
    delete [] edge_tag;
    delete [] parent;
    delete [] child;
}

int main() {
    Kruskal();
    system("pause");
    return 0;
}

