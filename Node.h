//
// Created by mount on 9/17/2026.
//

template <typename T>
class Node {
public:
    T* data;
    Node<T>* next;
    explicit Node(T* value) : data(value), next(nullptr) {}
};

//every node will have a T value with it and a pointer(called next) for the next node
//the first node is the head pointer and it's a node pointer