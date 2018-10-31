#ifndef LIST_NODE_HPP_
#define LIST_NODE_HPP_

#include <memory>

template<class T>
struct list_node {
    list_node(T value);
    ~list_node();

    T value;
    std::shared_ptr< list_node<T> > prev;
    std::shared_ptr< list_node<T> > next;
};

template<class T>
list_node<T>::list_node(T value): value(value)
{}

template<class T>
list_node<T>::~list_node() {
    delete next;
}

#endif // LIST_NODE_HPP_
