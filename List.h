
#pragma once
#include <memory>
template <typename T>
class List {
public:
    virtual ~List() = default;
    virtual void addFront(T* value) = 0;
    virtual void deleteFront() = 0;
    virtual bool search(T* value) const = 0;
    virtual void print() const = 0;
};
#include "ArrayList.h"
#include "LinkedList.h"
template <typename T>
std::unique_ptr<List<T>> makeList() {
     return std::make_unique<LinkedList<T>>();
     //return std::make_unique<ArrayList<T>>();
}