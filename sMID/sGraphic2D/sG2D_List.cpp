#include "sG2D_List.hpp"





static inline void* portMalloc(size_t size) {
    return pvPortMalloc(size);
}

static inline void portFree(void* ptr) {
    vPortFree(ptr);
}





// 添加节点到链表末尾
void sG2D_List::append(sG2D_ListNode* node) {
    // 如果是第一个元素
    if (this->head == nullptr) {
        this->head = node;
        node->prev = nullptr;
        node->next = nullptr;
    }
    // 不是第一个元素
    else {
        // 遍历到链表的末尾
        sG2D_ListNode* current = this->head;
        while (current->next != nullptr) {
            current = current->next;
        }
        // 在链表末尾添加新节点
        current->next = node;
        node->prev = current;
        node->next = nullptr;
    }
}

// 从链表中移除指定节点
void sG2D_List::remove(sG2D_ListNode* node) {
    if (node == nullptr) {
        return;
    }
    // 如果节点是头节点
    if (node == this->head) {
        this->head = node->next;
        if (this->head != nullptr) {
            this->head->prev = nullptr;
        }
    }
    else {
        // 更新前一个节点的 next 指针
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        }
        // 更新后一个节点的 prev 指针
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
    }
    // 断开节点的前后链接
    node->prev = nullptr;
    node->next = nullptr;
}

