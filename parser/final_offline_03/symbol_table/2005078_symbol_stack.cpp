#include <iostream>
#include "header_files/symbol_stack.h"

template <typename T>
Stack<T>::Stack() : head(nullptr), stackSize(0) {}

template <typename T>
Stack<T>::~Stack() {
    while (!empty()) {
        pop();
    }
}

template <typename T>
void Stack<T>::push(const T& value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    head = newNode;
    stackSize++;
}

template <typename T>
void Stack<T>::pop() {
    if (empty()) {
        std::cerr << "Stack is empty. Cannot perform pop operation." << std::endl;
        return;
    }

    Node* temp = head;
    head = head->next;
    delete temp;
    stackSize--;
}

template <typename T>
bool Stack<T>::empty() const {
    return stackSize == 0;
}

template <typename T>
size_t Stack<T>::size() const {
    return stackSize;
}

template <typename T>
T& Stack<T>::top() const {
    if (empty()) {
        throw std::runtime_error("Stack is empty. Cannot perform top operation.");
    }
    return head->data;
}