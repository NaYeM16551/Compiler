#ifndef Stack_H
#define Stack_H

#include <iostream>

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    size_t stackSize;

public:
    Stack();
    ~Stack();

    void push(const T& value);
    void pop();
    bool empty() const;
    size_t size() const;
    T& top() const;
};
#endif // Stack_H