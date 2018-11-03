#ifndef LIST_ITERATOR_HPP_
#define LIST_ITERATOR_HPP_

#include <iterator>
#include "list_node.hpp"

template<class T>
struct list_iterator {
    list_iterator(list_node<T>* initial, list_node<T>* sentinel);

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    bool operator==(list_iterator<T> const& rhs);
    bool operator!=(list_iterator<T> const& rhs);

    list_iterator<T>& operator++();
    list_iterator<T> operator++(int);
    list_iterator<T> operator+(std::size_t offset) const;

    list_iterator<T>& operator--();
    list_iterator<T> operator--(int);
    list_iterator<T> operator-(std::size_t offset) const;

    T& operator*();

private:
    list_node<T>* m_current;
    list_node<T>* m_sentinel;
};

template<class T>
list_iterator<T>::list_iterator(list_node<T>* initial, list_node<T>* sentinel):
    m_current(initial),
    m_sentinel(sentinel)
{}

template<class T>
bool list_iterator<T>::operator==(list_iterator<T> const& rhs) {
    return m_current == rhs.m_current;
}

template<class T>
bool list_iterator<T>::operator!=(list_iterator<T> const& rhs) {
    return !(*this == rhs);
}

template<class T>
list_iterator<T>& list_iterator<T>::operator++() {
    m_current = m_current->next;
    return *this;
}

template<class T>
list_iterator<T> list_iterator<T>::operator++(int) {
    auto old = *this;
    m_current = m_current->next;
    return old;
}

template<class T>
list_iterator<T> list_iterator<T>::operator+(std::size_t offset) const {
    auto result = m_current;

    while (result != m_sentinel && offset-- > 0) {
        result = result->next;
    }

    return list_iterator(result, m_sentinel);
}

template<class T>
list_iterator<T>& list_iterator<T>::operator--() {
    m_current = m_current->prev;
    return *this;
}

template<class T>
list_iterator<T> list_iterator<T>::operator--(int) {
    auto old = *this;
    m_current = m_current->prev;
    return old;
}

template<class T>
list_iterator<T> list_iterator<T>::operator-(std::size_t offset) const {
    auto result = m_current;

    while (result != m_sentinel && offset-- > 0) {
        result = result->prev;
    }

    return list_iterator(result, m_sentinel);
}

template<class T>
T& list_iterator<T>::operator*() {
    return m_current->value;
}

#endif // LIST_ITERATOR_HPP_
