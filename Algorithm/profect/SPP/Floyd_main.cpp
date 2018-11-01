/*
输入：

2
7 12
1 2 12
1 6 16
1 7 14
2 3 10
2 6 7
3 4 3
3 5 5
3 6 6
4 5 4
5 6 2
5 7 8
6 7 9 

*/
#include"Floyd.h"


//检验输入边数和顶点数的值是否有效，可以自己推算为啥：
//顶点数和边数的关系是：((Vexnum*(Vexnum - 1)) / 2) < edge
bool check(int Vexnum, int edge) {
    if (Vexnum <= 0 || edge <= 0 || ((Vexnum*(Vexnum - 1)) / 2) < edge)
        return false;
    return true;
}
int main() {
    int vexnum; int edge;
    cout << "输入图的种类：1代表有向图，2代表无向图" << endl;
    int kind;
    cin >> kind;
    //判读输入的kind是否合法
    while (1) {
        if (kind == 1 || kind == 2) {
            break;
        }
        else {
            cout << "输入的图的种类编号不合法，请重新输入：1代表有向图，2代表无向图" << endl;
            cin >> kind;
        }
    }

    cout << "输入图的顶点个数和边的条数：" << endl;
    cin >> vexnum >> edge;
    while (!check(vexnum, edge)) {
        cout << "输入的数值不合法，请重新输入" << endl;
        cin >> vexnum >> edge;
    }
    Graph_DG graph(vexnum, edge);
    graph.createGraph(kind);
    graph.print();
    graph.Floyd();
    graph.print_path();
    system("pause");
    return 0;
}
