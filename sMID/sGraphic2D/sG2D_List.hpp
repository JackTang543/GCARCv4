#pragma once

#include "sG2D.hpp"


template <typename T>
class sG2D_List{
public:
    struct sG2D_Node{
        T data;
        sG2D_Node* next;
        sG2D_Node* prev;

        sG2D_Node(const T& value);
        ~sG2D_Node();

        void* operator new(std::size_t size);
        void operator delete(void* ptr) noexcept;
    };

    sG2D_Node* head;
    sG2D_Node* tail;
    std::size_t sz;

public:
    sG2D_List();
    ~sG2D_List();

    void push_back(const T& value);
    void pop_back();
    void clear();

    std::size_t size() const;

};






