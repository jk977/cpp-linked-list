#ifndef LIST_ITERATOR_HPP_
#define LIST_ITERATOR_HPP_

#include <iterator>
#include "list_node.hpp"

template<class T>
struct list_iterator {
    list_iterator(list_node<T>* initial);

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    bool operator==(list_iterator<T> rhs);
    bool operator!=(list_iterator<T> rhs);

    list_iterator<T>& operator++();
    list_iterator<T> operator++(int);
    list_iterator<T> operator+(std::size_t offset) const;

    list_iterator<T>& operator--();
    list_iterator<T> operator--(int);
    list_iterator<T> operator-(std::size_t offset) const;

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
list_iterator<T> list_iterator<T>::operator++(int) {
    auto old = *this;
    current = current->next;
    return old;
}

template<class T>
list_iterator<T> list_iterator<T>::operator+(std::size_t offset) const {
    auto result = current;

    while (offset-- > 0) {
        result = result->next;
    }

    return result;
}

template<class T>
list_iterator<T>& list_iterator<T>::operator--() {
    current = current->prev;
    return *this;
}

template<class T>
list_iterator<T> list_iterator<T>::operator--(int) {
    auto old = *this;
    current = current->prev;
    return old;
}

template<class T>
list_iterator<T> list_iterator<T>::operator-(std::size_t offset) const {
    auto result = current;

    while (offset-- > 0) {
        result = result->prev;
    }

    return result;
}

template<class T>
T& list_iterator<T>::operator*() {
    return current->value;
}

#endif // LIST_ITERATOR_HPP_
