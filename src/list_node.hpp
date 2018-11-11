#ifndef LIST_NODE_HPP_
#define LIST_NODE_HPP_

template<class T>
struct list_node {
    list_node() = default;
    list_node(T val): value(val) {}
    ~list_node() { delete next; }

    T value;
    list_node<T>* prev;
    list_node<T>* next;
};

#endif // LIST_NODE_HPP_
