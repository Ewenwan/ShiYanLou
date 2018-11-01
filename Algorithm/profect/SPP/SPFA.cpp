#include"SFPA.h"

Graph::Graph(int vexnum, int edge) {
    //对顶点个数和边的条数进行赋值
    this->vexnum = vexnum;
    this->edge = edge;

    //为邻接矩阵开辟空间
    node = new Vnode[this->vexnum];
    dis = new Dis[this->vexnum];
    int i;
    //对邻接表进行初始化
    for (i = 0; i < this->vexnum; ++i) {
        node[i].data = "v" + to_string(i + 1);
        node[i].firstarc = NULL;
    }
}

Graph::~Graph() {
    int i;
    //释放空间，但是记住图中每个结点的链表也要一一释放
    ArcNode * p, *q;
    for (i = 0; i < this->vexnum; ++i) {
        //一定要注意这里，要判断该顶点的出度到底是否为空，不然会出错
        if (this->node[i].firstarc) {
            p = node[i].firstarc;
            while (p) {
                q = p->next;
                delete p;
                p = q;
            }
        }

    }
    delete [] node;
    delete [] dis;
}

// 判断我们每次输入的的边的信息是否合法
//顶点从1开始编号
bool Graph::check_edge_value(int start, int end, int weight) {
    if (start<1 || end<1 || start>vexnum || end>vexnum || weight < 0) {
        return false;
    }
    return true;
}

void Graph::print() {
    cout << "图的邻接表的打印：" << endl;
    int i;
    ArcNode *temp;
    //遍历真个邻接表
    for (i = 0; i < this->vexnum; ++i) {
        cout << node[i].data << " ";
        temp = node[i].firstarc;
        while (temp) {
            cout << "<"
                << node[i].data
                << ","
                << node[temp->adjvex].data
                << ">="
                << temp->weight
                << " ";
            temp = temp->next;
        }
        cout << "^" << endl;
    }
}

void Graph::createGraph(int kind) {
    //kind代表图的种类，2为无向图
    cout << "输入边的起点和终点以及各边的权重(顶点编号从1开始)：" << endl;
    int i;
    int start;
    int end;
    int weight;
    for (i = 0; i < this->edge; ++i) {
        cin >> start >> end >> weight;
        //判断输入的边是否合法
        while (!this->check_edge_value(start, end, weight)) {
            cout << "输入边的信息不合法，请重新输入：" << endl;
            cin >> start >> end >> weight;
        }
        ArcNode *temp = new ArcNode;
        temp->adjvex = end - 1;
        temp->weight = weight;
        temp->next = NULL;
        //如果该顶点依附的边为空，则从以第一个开始
        if (node[start - 1].firstarc == NULL) {
            node[start - 1].firstarc = temp;
        }
        else {//否则，则插入到该链表的最后一个位置
            ArcNode * now = node[start - 1].firstarc;
            //找到链表的最后一个结点
            while (now->next) {
                now = now->next;
            }
            now->next = temp;
        }
        //如果是无向图，则反向也要添加新的结点
        if (kind == 2) {
            //新建一个新的表结点
            ArcNode *temp_end = new ArcNode;
            temp_end->adjvex = start - 1;
            temp_end->weight = weight;
            temp_end->next = NULL;
            //如果该顶点依附的边为空，则从以第一个开始
            if (node[end - 1].firstarc == NULL) {
                node[end - 1].firstarc = temp_end;
            }
            else {//否则，则插入到该链表的最后一个位置
                ArcNode * now = node[end - 1].firstarc;
                //找到链表的最后一个结点
                while (now->next) {
                    now = now->next;
                }
                now->next = temp_end;
            }
        }

    }
}

bool Graph::SPFA(int begin) {
    bool  *visit;
    //visit用于记录是否在队列中
    visit = new bool[this->vexnum];
    int *input_queue_time;
    //input_queue_time用于记录某个顶点入队列的次数
    //如果某个入队列的次数大于顶点数vexnum，那么说明这个图有环，
    //没有最短路径，可以退出了
    input_queue_time = new int[this->vexnum];
    queue<int> s;  //队列，用于记录最短路径被改变的点

    /*
    各种变量的初始化
    */
    int i;
    for (i = 0; i < this->vexnum; i++) {
        visit[i] = false;
        input_queue_time[i] = 0;
        //路径开始都初始化为直接路径,长度都设置为无穷大
        dis[i].path = this->node[begin-1].data + "-->" + this->node[i].data;
        dis[i].weight = INT_MAX;
    }
    //首先是起点入队列，我们记住那个起点代表的是顶点编号，从1开始的
    s.push(begin - 1); 
    visit[begin - 1] = true; 
    ++input_queue_time[begin-1];
    //
    dis[begin - 1].path =this->node[begin - 1].data;
    dis[begin - 1].weight = 0;

    int temp;
    int res;
    ArcNode *temp_node;
    //进入队列的循环
    while (!s.empty()) {
        //取出队首的元素，并且把队首元素出队列
        temp = s.front(); s.pop();
        //必须要保证第一个结点不为空
        if (node[temp].firstarc)
        {
            temp_node = node[temp].firstarc;
            while (temp_node) {
                //如果边<temp,temp_node>的权重加上temp这个点的最短路径
                //小于之前temp_node的最短路径的长度，则更新
                //temp_node的最短路径的信息
                if (dis[temp_node->adjvex].weight > (temp_node->weight + dis[temp].weight)) {
                    //更新dis数组的信息
                    dis[temp_node->adjvex].weight = temp_node->weight + dis[temp].weight;
                    dis[temp_node->adjvex].path = dis[temp].path + "-->" + node[temp_node->adjvex].data;
                    //如果还没在队列中，加入队列，修改对应的信息
                    if (!visit[temp_node->adjvex]) {
                        visit[temp_node->adjvex] = true;
                        ++input_queue_time[temp_node->adjvex];
                        s.push(temp_node->adjvex);
                        if (input_queue_time[temp_node->adjvex] > this->vexnum) {
                            cout << "图中有环" << endl;
                            return false;
                        }
                    }
                }
                temp_node = temp_node->next;
            }
        }   
    }

    //打印最短路径

    return true;
}

void Graph::print_path(int begin) {
    cout << "以顶点" << this->node[begin - 1].data
        << "为起点，到各个顶点的最短路径的信息：" << endl;
    int i;
    for (i = 0; i < this->vexnum; ++i) {
        if (dis[i].weight == INT_MAX) {
            cout << this->node[begin - 1].data << "---"
                << this->node[i].data
                << "  无最短路径，这两个顶点不连通" << endl;
        }
        else
        {
            cout << this->node[begin - 1].data << "---"
                << this->node[i].data
                << "  weight: "
                << dis[i].weight
                << "  path: "
                << dis[i].path
                << endl;
        }

    }
}
