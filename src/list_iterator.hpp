#ifndef LIST_ITERATOR_HPP_
#define LIST_ITERATOR_HPP_

#include <iterator>
#include "list_node.hpp"

namespace dsa {

template<class T>
struct list_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    list_iterator(list_node<value_type>* initial): m_current(initial) {}

    reference operator*() { return m_current->value; }
    pointer operator->() { return &**this; }

    bool operator==(list_iterator<value_type> const& rhs) {
        return m_current == rhs.m_current;
    }

    bool operator!=(list_iterator<value_type> const& rhs) {
        return !(*this == rhs);
    }

    list_iterator<value_type>& operator++() {
        m_current = m_current->next;
        return *this;
    }

    list_iterator<value_type> operator++(int) {
        auto old = *this;
        ++*this;
        return old;
    }

    list_iterator<value_type>& operator--() {
        m_current = m_current->prev;
        return *this;
    }

    list_iterator<value_type> operator--(int) {
        auto old = *this;
        --*this;
        return old;
    }

private:
    list_node<value_type>* m_current;
};

}

#endif // LIST_ITERATOR_HPP_
