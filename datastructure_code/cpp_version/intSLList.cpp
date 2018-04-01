//************************  intSLList.cpp  **************************
// 单链表实现的类  
#include <iostream.h>
#include "intSLList.h"

IntSLList::~IntSLList() {// 默认析构函数 
    for (IntSLLNode *p; !isEmpty(); ) {
        p = head->next;//指向下一个节点 
        delete head;//删除当前节点 
        head = p;   //把头结点 移到下一个节点 
    }
}

// 从头部添加 节点 
void IntSLList::addToHead(int el) {
    head = new IntSLLNode(el,head);
	//新建了一个节点 其内容为 el，而指向的节点为 原来的头结点 返回一个指针 作为新的头结点 
    if (tail == 0)// tail未初始化的话 也赋值 
       tail = head;
}

//
//   [tail]  ----> |  10 |  
//                 |  \  | 为 tail->next  
//
//                 |  10 |  
//                 |     | -----> | 30| <------ [tail] 
//                                | \ | 
// 从尾部添加 节点
void IntSLList::addToTail(int el) {
    if (tail != 0) {      // 非空链表  
         tail->next = new IntSLLNode(el);// 新建一个节点 接在tail后面 
         tail = tail->next;
    }//空链表 
    else head = tail = new IntSLLNode(el);
}

int IntSLList::deleteFromHead() {
    int el = head->info;
    IntSLLNode *tmp = head;
    if (head == tail)     // if only one node on the list;
         head = tail = 0;
    else head = head->next;
    delete tmp;
    return el;
}

int IntSLList::deleteFromTail() {
    int el = tail->info;
    if (head == tail) {   // if only one node on the list;
         delete head;
         head = tail = 0;
    }
    else {                // if more than one node in the list,
         IntSLLNode *tmp; // find the predecessor of tail;
         for (tmp = head; tmp->next != tail; tmp = tmp->next);
         delete tail;
         tail = tmp;      // the predecessor of tail becomes tail;
         tail->next = 0;
    }
    return el;
}

void IntSLList::deleteNode(int el) {
    if (head != 0)                     // if non-empty list;
         if (head == tail && el == head->info) { // if only one
              delete head;                       // node on the list;
              head = tail = 0;
         }
         else if (el == head->info) {  // if more than one node on the list
              IntSLLNode *tmp = head;
              head = head->next;
              delete tmp;              // and old head is deleted;
         }
         else {                        // if more than one node in the list
              IntSLLNode *pred, *tmp;
              for (pred = head, tmp = head->next; // and a non-head node
                   tmp != 0 && !(tmp->info == el);// is deleted;
                   pred = pred->next, tmp = tmp->next);
              if (tmp != 0) {
                   pred->next = tmp->next;
                   if (tmp == tail)
                      tail = pred;
                   delete tmp;
              }
         }
}

bool IntSLList::isInList(int el) const {
    IntSLLNode *tmp;
    for (tmp = head; tmp != 0 && !(tmp->info == el); tmp = tmp->next);
    return tmp != 0;
}

void IntSLList::printAll() const {
    for (IntSLLNode *tmp = head; tmp != 0; tmp = tmp->next)
        cout << tmp->info << " ";
	cout << endl;
}

