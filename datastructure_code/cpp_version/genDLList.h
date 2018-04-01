#ifndef DOUBLY_LINKED_LIST
#define DOUBLY_LINKED_LIST

template<class T>
// 双向链表 节点 包含信息  和 两个地址指针 分别指向 前驱节点 和 后继节点 
class DLLNode {
public:
    DLLNode() {
        next = prev = 0;
    }
    DLLNode(const T& el, DLLNode<T> *n = 0, DLLNode<T> *p = 0) {
        info = el; next = n; prev = p;
    }
    T info;// 包含信息 
    DLLNode<T> *next, *prev;//  两个地址指针 分别指向 前驱节点 和 后继节点 
};

template<class T>
class DoublyLinkedList {
public:
	// 默认构造函数 
    DoublyLinkedList() {
        head = tail = 0;
    }
    // 默认析构函数 
    void addToDLLTail(const T&);
    T deleteFromDLLTail();
    ~DoublyLinkedList() {
        clear();
    }
    
    bool isEmpty() const {
        return head == 0;
    }
    void clear();
    void setToNull() {
        head = tail = 0;
    }
    void addToDLLHead(const T&);
    T deleteFromDLLHead();
    T& firstEl();
    T* find(const T&) const;
protected:
    DLLNode<T> *head, *tail;
    friend ostream& operator<<(ostream& out, const DoublyLinkedList<T>& dll) {
        for (DLLNode<T> *tmp = dll.head; tmp != 0; tmp = tmp->next)
            out << tmp->info << ' ';
        return out;
    }
};

// 头部添加 
template<class T>
void DoublyLinkedList<T>::addToDLLHead(const T& el) {
    if (head != 0) {
         head = new DLLNode<T>(el,head,0);
         head->next->prev = head;
    }
    else head = tail = new DLLNode<T>(el);
}

template<class T>
void DoublyLinkedList<T>::addToDLLTail(const T& el) {
    if (tail != 0) {
         tail = new DLLNode<T>(el,0,tail);
         tail->prev->next = tail;
    }
    else head = tail = new DLLNode<T>(el);
}

template<class T>
T DoublyLinkedList<T>::deleteFromDLLHead() {
    T el = head->info;
    if (head == tail) { // if only one DLLNode on the list;
         delete head;
         head = tail = 0;
    }
    else {              // if more than one DLLNode in the list;
         head = head->next;
         delete head->prev;
         head->prev = 0;
    }
    return el;
}

template<class T>
T DoublyLinkedList<T>::deleteFromDLLTail() {
    T el = tail->info;
    if (head == tail) { // if only one DLLNode on the list;
         delete head;
         head = tail = 0;
    }
    else {              // if more than one DLLNode in the list;
         tail = tail->prev;
         delete tail->next;
         tail->next = 0;
    }
    return el;
}

template <class T>
T* DoublyLinkedList<T>::find(const T& el) const {
    DLLNode<T> *tmp = head;
    for ( ; tmp != 0 && !(tmp->info == el);  // overloaded ==
         tmp = tmp->next);
    if (tmp == 0)
         return 0;
    else return &tmp->info;
}

template<class T>
T& DoublyLinkedList<T>::firstEl() {
    return head->info;
}

template<class T>
void DoublyLinkedList<T>::clear() {
    for (DLLNode<T> *tmp; head != 0; ) {
        tmp = head;
        head = head->next;
        delete tmp;
    }
}

#endif
