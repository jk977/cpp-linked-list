#ifndef LIST_ITERATOR_HPP_
#define LIST_ITERATOR_HPP_

#include "list_node.hpp"

template<class T>
struct list_iterator {
    list_iterator(list_node<T>* initial);

    bool operator==(list_iterator<T> rhs);
    bool operator!=(list_iterator<T> rhs);
    list_iterator<T>& operator++();
    T& operator*();

private:
    list_node<T>* current;
};

template<class T>
list_iterator<T>::list_iterator(list_node<T>* initial): current(initial)
{}

template<class T>
bool list_iterator<T>::operator==(list_iterator<T> rhs) {
    return current == rhs.current;
}

template<class T>
bool list_iterator<T>::operator!=(list_iterator<T> rhs) {
    return !(*this == rhs);
}

template<class T>
list_iterator<T>& list_iterator<T>::operator++() {
    current = current->next;
    return *this;
}

template<class T>
T& list_iterator<T>::operator*() {
    return current->value;
}

#endif // LIST_ITERATOR_HPP_
