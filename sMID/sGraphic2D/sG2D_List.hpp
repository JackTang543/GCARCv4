#pragma once

#include "sG2D.hpp"


#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))






// 节点类，数据可以继承此类，然后数据就可以变成链表节点
class sG2D_ListNode{
    // 只有派生类和友元类才能访问链表节点信息（对用户隐藏）
public:
    // 指向上一个节点
    sG2D_ListNode* prev;
    // 指向下一个节点
    sG2D_ListNode* next;

    // 构造函数，把上下节点指针设置为 nullptr
    sG2D_ListNode() : prev(nullptr), next(nullptr) {}

};

// 链表类，继承自 ListNode 的数据可以通过这个类的方法，把数据变成这个链表的节点
// 同样的，一份数据可以被多个节点指向
class sG2D_List{
public:
    sG2D_List() : head(nullptr) {}

    ~sG2D_List() {}

    void append(sG2D_ListNode* node);

    void remove(sG2D_ListNode* node);

private:
    // 如果为空链表，这个指针就是 nullptr，如果不是空链表，这个指针指向链表第一个元素
    sG2D_ListNode* head;
};


