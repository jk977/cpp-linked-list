#ifndef LIST_HPP_
#define LIST_HPP_

#include <cstdlib>
#include <utility>

#include "list_node.hpp"
#include "list_iterator.hpp"

template<class T>
class list {
public:
    list();
    ~list();

    using iterator = list_iterator<T>;

    iterator begin();
    iterator end();

    void push_front(T val);
    void push_back(T val);
    void insert(std::size_t index, T val);

    T get_front() const;
    T get_last() const;
    T get(std::size_t index) const;

    std::size_t length() const;
    bool empty() const;

private:
    void insert_empty(T val);

    std::size_t m_length;
    list_node<T>* m_sentinel;
};

template<class T>
list<T>::list(): m_length(0) {
    m_sentinel = new list_node(T());
}

template<class T>
list<T>::~list() {
    auto& tail = m_sentinel->prev;

    if (tail != nullptr) {
        // sever tail from sentinel to prevent double free
        tail->next = nullptr;
    }

    delete m_sentinel;
}

template<class T>
typename list<T>::iterator list<T>::begin() {
    return list_iterator(m_sentinel->next);
}

template<class T>
typename list<T>::iterator list<T>::end() {
    return list_iterator(m_sentinel);
}

template<class T>
void list<T>::insert_empty(T val) {
    auto node = new list_node(val);

    m_sentinel->next = node;
    m_sentinel->prev = node;
    node->next = m_sentinel;
    node->prev = m_sentinel;
}

template<class T>
void list<T>::push_front(T val) {
    if (length() == 0) {
        insert_empty(val);
    } else {
        auto old_head = m_sentinel->next;
        auto new_head = new list_node(val);

        m_sentinel->next = new_head;
        new_head->prev = m_sentinel;
        new_head->next = old_head;
        old_head->prev = new_head;
    }

    m_length++;
}

template<class T>
void list<T>::push_back(T val) {
    if (length() == 0) {
        insert_empty(val);
    } else {
        auto old_tail = m_sentinel->prev;
        auto new_tail = new list_node(val);

        m_sentinel->prev = new_tail;
        new_tail->prev = old_tail;
        new_tail->next = m_sentinel;
        old_tail->next = new_tail;
    }

    m_length++;
}

template<class T>
std::size_t list<T>::length() const {
    return m_length;
}

template<class T>
bool list<T>::empty() const {
    return length() == 0;
}

#endif // LIST_HPP_
