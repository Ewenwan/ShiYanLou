// 双向链表实现===================

#ifndef __LIBS_LIST_H__
#define __LIBS_LIST_H__

#ifndef __ASSEMBLER__

#include <defs.h>
// 包含一些简单的类型定义
//  typedef int bool;     // 布尔量
// typedef char int8_t; // -128~127
// typedef unsigned char uint8_t; // 0~255
// typedef short int16_t;                // 
// typedef unsigned short uint16_t; // 0~65535
// typedef int int32_t;
// typedef unsigned int uint32_t;
// typedef long long int64_t;
// typedef unsigned long long uint64_t;
// typedef int32_t intptr_t;
// typedef uint32_t uintptr_t;  // 32为地址指针

// 双向链表节点
struct list_entry {
    struct list_entry *prev, *next;
};
typedef struct list_entry list_entry_t;

// attribute((always_inline))的意思是强制内联，
// 所有加了attribute((always_inline))的函数在被调用时不会被编译成函数调用而是直接扩展到调用函数体内。

// 链表节点初始化=======
static inline void list_init(list_entry_t *elm) __attribute__((always_inline));

// 默认节点插入为 在 指定节点之后插入新节点========
static inline void list_add(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));// 

// 在 listelm 节点之前插入 一个新节点 elm ==========
static inline void list_add_before(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline)); 

// 在 listelm 节点之后 插入 一个新节点 elm ==========
static inline void list_add_after(list_entry_t *listelm, list_entry_t *elm) __attribute__((always_inline));
// 删除节点 listelm  ======
static inline void list_del(list_entry_t *listelm) __attribute__((always_inline));
// 删除节点 listelm ，并将其内存初始化为指向自身======
static inline void list_del_init(list_entry_t *listelm) __attribute__((always_inline));
// 测试链表 list 是否为空，如果其后继指向自身，则为空?还要判断其前驱也指向本身？======
static inline bool list_empty(list_entry_t *list) __attribute__((always_inline));
//  获取链表的 下一个节点 后继======
static inline list_entry_t *list_next(list_entry_t *listelm) __attribute__((always_inline));
// 获取链表的 前一个节点 前驱=====
static inline list_entry_t *list_prev(list_entry_t *listelm) __attribute__((always_inline));
// 在节点 prev  和 next 之间添加 一个新节点 elm=====
static inline void __list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) __attribute__((always_inline));
// 将两个节点连接在一起====
static inline void __list_del(list_entry_t *prev, list_entry_t *next) __attribute__((always_inline));

/* *
 * list_init - initialize a new entry              初始化一个新的节点入口
 * @elm:        new entry to be initialized 一个新的双向链表
 * */
static inline void
list_init(list_entry_t *elm) {
    elm->prev = elm->next = elm;// 前节点指针 和 后节点指针 初始化为指向自己
}

/* * 默认节点插入为 在 指定节点之后插入新节点========
 * list_add -  在 listelm节点 之后插入一个新节点 elm
 * @listelm:    被插入的节点
 * @elm:         要插入的新节点
 * */
static inline void
list_add(list_entry_t *listelm, list_entry_t *elm) {
    list_add_after(listelm, elm);
}

/* *
 * 在 listelm 节点之前插入 一个新节点 elm
 * @listelm:    被插入的节点
 * @elm:         要插入的新节点
 * */
static inline void
list_add_before(list_entry_t *listelm, list_entry_t *elm) {
// 在  listelm->prev 节点 和 listelm 节点之间 插入 一个新的节点elm,
    __list_add(elm, listelm->prev, listelm);
}

/* *
 * list_add_after - 在 listelm节点 之后插入一个新节点 elm
 * @ listelm:    被插入的节点
 * @ elm:         要插入的新节点
 * */
static inline void
list_add_after(list_entry_t *listelm, list_entry_t *elm) {
  // 在  listelm 节点 和 listelm->next 节点之间 插入 一个新的节点 elm,
    __list_add(elm, listelm, listelm->next);
}

/* *
 * list_del - 删除节点 listelm -将其 前驱和后继连接在一起
 * @listelm:  需要被删除的节点
 * */
static inline void
list_del(list_entry_t *listelm) {
// 将其 前驱和后继连接在一起, 自身就访问不到了，但是其两个指针还是指向了其他的内存
    __list_del(listelm->prev, listelm->next);
}

/* *
 * list_del_init - 删除节点 listelm ，并将其内存初始化为指向自身
 * @listelm:    需要被删除的节点
 * */
static inline void
list_del_init(list_entry_t *listelm) {
    list_del(listelm);// 删除该节点
    list_init(listelm);// 将其前驱指针 和 后继指针初始化为自身的地址
}

/* *
 * list_empty - 测试链表 list 是否为空，如果其后继指向自身，则为空?还要判断其前驱也指向本身？
 * @list:       需要测试的链表
 * */
static inline bool
list_empty(list_entry_t *list) {
    return list->next == list;// 后继指向自己?    return (list->next == list)&(list->prev == list); // 有点小问题
}

/* *
 * list_next - 获取链表的下一个 节点 后继
 * @listelm:    the list head
 **/
static inline list_entry_t *
list_next(list_entry_t *listelm) {
    return listelm->next;
}

/* *
 * list_prev - 获取链表的 前一个节点 前驱
 * @listelm:    the list head
 **/
static inline list_entry_t *
list_prev(list_entry_t *listelm) {
    return listelm->prev;
}

/* *
 *  在 prev 节点 和 next 节点之间 插入 一个新的节点 elm,
 * 函数第一个形参是被插入的节点
 * 函数第二个参数是最左边的节点
 * 函数第三个参数是最右边的节点
 * */
static inline void
__list_add(list_entry_t *elm, list_entry_t *prev, list_entry_t *next) {
    prev->next = next->prev = elm; // prev节点的后继 和 next的前驱 都指向新元素 elm
    elm->next = next;// 新节点的后继 指向 next节点
    elm->prev = prev;// 新节点的前驱 指向 prev节点
}

/* *
 * 删除节点----连接两个节点，他们之间的节点就被删除了
 * */
static inline void
__list_del(list_entry_t *prev, list_entry_t *next) {
    prev->next = next;
    next->prev = prev;
}

#endif /* !__ASSEMBLER__ */

#endif /* !__LIBS_LIST_H__ */

