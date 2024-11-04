#include "sG2D_List.hpp"





static inline void* portMalloc(std::size_t size) {
    return pvPortMalloc(size);
}

static inline void portFree(void* ptr) {
    vPortFree(ptr);
}


//==================Node成员函数实现==================

template <typename T>
sG2D_List<T>::sG2D_Node::sG2D_Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}

template <typename T>
sG2D_List<T>::sG2D_Node::~sG2D_Node() {
    // 如果 T 是指针类型，且需要手动释放内存，可以在这里处理
}

// 重载 new 操作符
template <typename T>
void* sG2D_List<T>::sG2D_Node::operator new(std::size_t size) {
    void* ptr = portMalloc(size);
    if (!ptr) {
        // 根据需要处理内存分配失败的情况
        // 可以抛出异常或其他方式处理
        // for (;;);  // 死循环等待
        sDBG_Debug_Warning("sG2D_Node内存分配失败");
    }
    return ptr;
}

// 重载 delete 操作符
template <typename T>
void sG2D_List<T>::sG2D_Node::operator delete(void* ptr) noexcept {
    portFree(ptr);
}


// ================== List 成员函数实现 ==================

template <typename T>
sG2D_List<T>::sG2D_List() : head(nullptr), tail(nullptr), sz(0) {}

template <typename T>
sG2D_List<T>::~sG2D_List() {
    clear();
}

template <typename T>
void sG2D_List<T>::push_back(const T& value) {
    sG2D_Node* node = new sG2D_Node(value);
    if (!node) {
        return;
    }
    if (!head) {
        head = tail = node;
    } else {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
    ++sz;
}

template <typename T>
void sG2D_List<T>::pop_back() {
    if (!tail) return;
    sG2D_Node* node = tail;
    tail = tail->prev;
    if (tail)
        tail->next = nullptr;
    else
        head = nullptr;
    delete node;
    --sz;
}

template <typename T>
void sG2D_List<T>::clear() {
    while (head) {
        pop_back();
    }
}

template <typename T>
std::size_t sG2D_List<T>::size() const {
    return sz;
}


template class sG2D_List<int>;



