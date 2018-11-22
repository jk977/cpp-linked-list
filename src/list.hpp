#ifndef LIST_HPP_
#define LIST_HPP_

#include <cstdlib>
#include <optional>
#include <iterator>
#include <functional>

#include "list_node.hpp"
#include "list_iterator.hpp"

namespace dsa {

template<class T>
class list {
public:
    list();
    ~list();

    using map_fn = std::function<T(T const&)>;

    using iterator = detail::list_iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    iterator begin();
    iterator end();

    reverse_iterator rbegin();
    reverse_iterator rend();

    void map(map_fn f);

    void push_front(T val);
    void push_back(T val);
    void insert(T val, std::size_t index);

    std::optional<T> pop_front();
    std::optional<T> pop_back();
    std::optional<T> pop(std::size_t index);

    std::optional<T> get_front() const;
    std::optional<T> get_back() const;
    std::optional<T> get(std::size_t index) const;

    std::size_t length() const;
    bool empty() const;
    void clear();

    T& operator[](std::size_t index);

private:
    using node_t = detail::list_node<T>;

    node_t* node_at(std::size_t index) const;
    void insert_empty(T val);
    void insert_middle(T val, std::size_t index);

    std::size_t m_length;
    node_t* m_sentinel;
};

template<class T>
list<T>::list(): m_length(0) {
    // list is cyclical -- all nodes will always have a next and a prev
    auto node = new node_t();
    node->next = node;
    node->prev = node;

    m_sentinel = node;
}

template<class T>
list<T>::~list() {
    clear();
    m_sentinel->next = nullptr;
    delete m_sentinel;
}

template<class T>
typename list<T>::iterator list<T>::begin() {
    return iterator(m_sentinel->next);
}

template<class T>
typename list<T>::iterator list<T>::end() {
    return iterator(m_sentinel);
}

template<class T>
typename list<T>::reverse_iterator list<T>::rbegin() {
    return reverse_iterator(end());
}

template<class T>
typename list<T>::reverse_iterator list<T>::rend() {
    return reverse_iterator(m_sentinel->next);
}

template<class T>
void list<T>::map(list<T>::map_fn f) {
    for (auto& elem : *this) {
        elem = f(elem);
    }
}

template<class T>
void list<T>::insert_empty(T val) {
    // insert into an empty list
    auto node = new node_t(val);

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
        auto new_head = new node_t(val);

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
        auto new_tail = new node_t(val);

        m_sentinel->prev = new_tail;
        new_tail->prev = old_tail;
        new_tail->next = m_sentinel;
        old_tail->next = new_tail;
    }

    m_length++;
}

template<class T>
typename list<T>::node_t* list<T>::node_at(std::size_t index) const {
    auto current = m_sentinel->next;
    std::size_t i = 0;

    // stop at sentinel to prevent wrapping around to the beginning
    // when index > length()
    while (current != m_sentinel && i < index) {
        current = current->next;
        i++;
    }

    // return nullptr instead of sentinel if index was out of bounds
    return (current != m_sentinel) ?
        current :
        nullptr;
}

template<class T>
void list<T>::insert_middle(T val, std::size_t index) {
    // insert at a place other than the front or back
    auto new_node = new node_t(val);
    auto target = node_at(index);
    auto prev = target->prev;

    prev->next = new_node;
    new_node->prev = prev;

    new_node->next = target;
    target->prev = new_node;

    m_length++;
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
std::optional<T> list<T>::pop_front() {
    return pop(0);
}

template<class T>
std::optional<T> list<T>::pop_back() {
    return pop(length() - 1);
}

template<class T>
std::optional<T> list<T>::pop(std::size_t index) {
    if (m_length != 0) {
        m_length--;
    }

    return detail::pop_node(node_at(index));
}

template<class T>
std::optional<T> list<T>::get_front() const {
    return get(0);
}

template<class T>
std::optional<T> list<T>::get_back() const {
    return get(length() - 1);
}

template<class T>
std::optional<T> list<T>::get(std::size_t index) const {
    return detail::value_of(node_at(index));
}

template<class T>
std::size_t list<T>::length() const {
    return m_length;
}

template<class T>
bool list<T>::empty() const {
    return length() == 0;
}

template<class T>
void list<T>::clear() {
    // resets list to initial state (length() == 0)

    if (length() > 0) {
        m_sentinel->prev->next = nullptr; // break cycle to prevent double free
        delete m_sentinel->next;

        m_sentinel->next = m_sentinel;
        m_sentinel->prev = m_sentinel;
        m_length = 0;
    }
}

template<class T>
T& list<T>::operator[](std::size_t index) {
    if (index >= length()) {
        throw std::out_of_range("Index is greater than the list length.");
    }

    return node_at(index)->value;
}

}

#endif // LIST_HPP_
