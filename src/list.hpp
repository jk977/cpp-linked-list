#ifndef LIST_HPP_
#define LIST_HPP_

#include <cstdlib>
#include <utility>
#include <optional>

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
    void insert(T val, std::size_t index);

    std::optional<T> pop_front();
    std::optional<T> pop_back();
    void remove(std::size_t index);

    std::optional<T> get_front() const;
    std::optional<T> get_back() const;
    std::optional<T> get(std::size_t index) const;

    std::size_t length() const;
    bool empty() const;

private:
    std::optional< list_node<T>* > node_at(std::size_t index) const;
    void insert_empty(T val);
    void insert_middle(T val, std::size_t index);

    std::size_t m_length;
    list_node<T>* m_sentinel;
};

template<class T>
list<T>::list(): m_length(0) {
    m_sentinel = new list_node<T>();
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
std::optional< list_node<T>* > node_from_offset(std::size_t index, list_node<T>* start) {
    if (start == nullptr) {
        return std::nullopt;
    } else if (index == 0) {
        return std::make_optional(start);
    }

    return node_from_offset(index-1, start->next);
}

template<class T>
std::optional< list_node<T>* > list<T>::node_at(std::size_t index) const {
    return node_from_offset(index, m_sentinel->next);
}

template<class T>
void list<T>::insert_middle(T val, std::size_t index) {
    auto new_node = new list_node(val);
    auto target = *node_at(index);
    auto prev = target->prev;

    prev->next = new_node;
    new_node->prev = prev;

    new_node->next = target;
    target->prev = new_node;
}

template<class T>
void list<T>::insert(T val, std::size_t index) {
    if (index == 0) {
        push_front(val);
    } else if (index == length()) {
        push_back(val);
    } else {
        insert_middle(val, index);
    }
}

template<class T>
std::optional<T> value_of(list_node<T>* node) {
    return (node != nullptr) ?
        std::make_optional(node->value) :
        std::nullopt;
}

template<class T>
std::optional<T> pop(list_node<T>* node) {
    if (node == nullptr) {
        return std::nullopt;
    }

    auto node_before = node->prev;
    auto node_after = node->next;
    auto value = value_of(node);

    node->next = nullptr;
    node_before->next = node_after;
    node_after->prev = node_before;
    delete node;

    return value;
}

template<class T>
std::optional<T> list<T>::pop_front() {
    return pop(m_sentinel->next);
}

template<class T>
std::optional<T> list<T>::pop_back() {
    return pop(m_sentinel->prev);
}

template<class T>
void list<T>::remove(std::size_t index) {
    pop( node_at(index).value_or(nullptr) );
}

template<class T>
std::optional<T> list<T>::get_front() const {
    return value_of(m_sentinel->next);
}

template<class T>
std::optional<T> list<T>::get_back() const {
    return value_of(m_sentinel->prev);
}

template<class T>
std::optional<T> list<T>::get(std::size_t index) const {
    return value_of( node_at(index).value_or(nullptr) );
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
