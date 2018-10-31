#ifndef LIST_NODE_HPP_
#define LIST_NODE_HPP_

template<class T>
struct list_node {
    list_node() = default;
    list_node(T val);
    ~list_node();

    T value;
    list_node<T>* prev;
    list_node<T>* next;
};

template<class T>
list_node<T>::list_node(T val): value(val)
{}

template<class T>
list_node<T>::~list_node() {
    delete next;
}

#endif // LIST_NODE_HPP_
