#include <iostream>
#include "list.hpp"

int main() {
    list<int> l;
    l.push_front(3);
    l.push_back(4);
    l.push_front(2);
    l.push_front(1);
    l.push_back(5);

    for (int i : l) {
        std::cout << i << '\n';
    }

    return 0;
}
