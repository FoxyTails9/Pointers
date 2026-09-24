#include <iostream>
#include "List.h"
#include "Data.h"
int main() {
    std::unique_ptr<List<int>> list = makeList<int>();
    list->addFront(new int(10));
    list->addFront(new int(20));
    list->addFront(new int(30));
    list->print();
    int key = 20;
    std::cout << "search(20): "
    << (list->search(&key) ? "found" : "not found") << std::endl;
    list->deleteFront();
    list->print();
    std::unique_ptr<List<Data>> roster = makeList<Data>();
    roster->addFront(new Data(1, "Alice"));
    roster->addFront(new Data(2, "Bilal"));
    roster->addFront(new Data(3, "Chen"));
    roster->print();
    Data query(2, "");
    std::cout << "search(id 2): "
    << (roster->search(&query) ? "found" : "not found")
    << std::endl;
    return 0;
}