/*
 * Thread-safe linked list implementation.
 */

#ifndef LIST_HPP_
#define LIST_HPP_

#include <cstdint>
#include <cassert>
#include <optional>
#include <functional>

#include <mutex>
#include <shared_mutex>

#include "list_node.hpp"

namespace dsa {

template<class T>
class list {
public:
    list();
    ~list();

    using modify_fn = std::function<T(T const&)>;
    using index_type = std::int64_t;

    void push_front(T val);
    void push_back(T val);
    void insert(T val, index_type index);

    std::optional<T> pop_front();
    std::optional<T> pop_back();
    std::optional<T> pop(index_type index);

    std::optional<T> get_front() const;
    std::optional<T> get_back() const;
    std::optional<T> get(index_type index) const;

    void map(modify_fn const& f);

    void modify_front(modify_fn const& f);
    void modify_back(modify_fn const& f);
    void modify(index_type index, modify_fn const& f);

    index_type length() const;
    bool empty() const;
    void clear();

private:
    using node_t = detail::list_node<T>;

    void insert_empty(T val);
    void insert_middle(T val, index_type index);

    node_t*          node_at(index_type index) const;
    std::optional<T> pop_at(index_type index);
    void             modify_at(index_type index, modify_fn const& f);

    node_t* m_sentinel;
    std::shared_mutex mutable m_mutex;
    index_type m_length;
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
void list<T>::insert_empty(T val) {
    assert( m_length == 0 );
    assert( !m_mutex.try_lock() );

    auto node = new node_t(val);

    m_sentinel->next = node;
    m_sentinel->prev = node;
    node->next = m_sentinel;
    node->prev = m_sentinel;
}

template<class T>
void list<T>::push_front(T val) {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        insert_empty(val);
    } else {
        auto old_head = m_sentinel->next;
        auto new_head = new node_t(val);

        m_sentinel->next = new_head;
        new_head->prev = m_sentinel;
        new_head->next = old_head;
        old_head->prev = new_head;
    }

    ++m_length;
}

template<class T>
void list<T>::push_back(T val) {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        insert_empty(val);
    } else {
        auto old_tail = m_sentinel->prev;
        auto new_tail = new node_t(val);

        m_sentinel->prev = new_tail;
        new_tail->prev = old_tail;
        new_tail->next = m_sentinel;
        old_tail->next = new_tail;
    }

    ++m_length;
}

template<class T>
void list<T>::insert_middle(T val, list<T>::index_type index) {
    // insert at a place other than the front or back
    std::unique_lock lock(m_mutex);

    auto new_node = new node_t(val);
    auto target = node_at(index);
    auto prev = target->prev;

    prev->next = new_node;
    new_node->prev = prev;

    new_node->next = target;
    target->prev = new_node;

    ++m_length;
}

template<class T>
void list<T>::insert(T val, list<T>::index_type index) {
    if (index == 0) {
        push_front(val);
    } else if (index == length()) {
        push_back(val);
    } else {
        insert_middle(val, index);
    }
}

template<class T>
typename list<T>::node_t* list<T>::node_at(list<T>::index_type index) const {
    assert( !m_mutex.try_lock() );

    auto current = m_sentinel->next;
    index_type i = 0;

    // stop at sentinel to prevent wrapping around to the beginning
    // when index > length()
    while (current != m_sentinel && i < index) {
        current = current->next;
        ++i;
    }

    // return nullptr instead of sentinel if index was out of bounds
    return (current != m_sentinel) ?
        current :
        nullptr;
}

template<class T>
std::optional<T> list<T>::pop_at(list<T>::index_type index) {
    assert( !m_mutex.try_lock() );

    if (m_length > 0) {
        --m_length;
    }

    return detail::pop_node(node_at(index));
}

template<class T>
std::optional<T> list<T>::pop_front() {
    std::unique_lock lock(m_mutex);
    return pop_at(0);
}

template<class T>
std::optional<T> list<T>::pop_back() {
    std::unique_lock lock(m_mutex);
    return pop_at(m_length-1);
}

template<class T>
std::optional<T> list<T>::pop(list<T>::index_type index) {
    std::unique_lock lock(m_mutex);
    return pop_at(index);
}

template<class T>
std::optional<T> list<T>::get_front() const {
    return get(0);
}

template<class T>
std::optional<T> list<T>::get_back() const {
    std::shared_lock lock(m_mutex);
    return get(m_length-1);
}

template<class T>
std::optional<T> list<T>::get(list<T>::index_type index) const {
    std::shared_lock lock(m_mutex);
    return detail::value_of(node_at(index));
}

template<class T>
void list<T>::map(list<T>::modify_fn const& f) {
    std::unique_lock lock(m_mutex);

    auto current = m_sentinel->next;

    while (current != m_sentinel) {
        current->value = f(current->value);
        current = current->next;
    }
}

template<class T>
void list<T>::modify_at(list<T>::index_type index, list<T>::modify_fn const& f) {
    // to prevent data races while allowing mutation of specific elements,
    // elements are modified by providing an index as well as a function that
    // takes the element and returns the new value for the element.
    assert( !m_mutex.try_lock() );

    auto target = node_at(index);

    if (target != nullptr) {
        target->value = f(target->value);
    }
}

template<class T>
void list<T>::modify_front(list<T>::modify_fn const& f) {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        return;
    }

    modify_at(0, f);
}

template<class T>
void list<T>::modify_back(list<T>::modify_fn const& f) {
    std::unique_lock lock(m_mutex);
    modify_at(m_length-1, f);
}

template<class T>
void list<T>::modify(list<T>::index_type index, list<T>::modify_fn const& f) {
    std::unique_lock lock(m_mutex);
    modify_at(index, f);
}

template<class T>
typename list<T>::index_type list<T>::length() const {
    std::shared_lock lock(m_mutex);
    return m_length;
}

template<class T>
bool list<T>::empty() const {
    std::shared_lock lock(m_mutex);
    return m_length == 0;
}

template<class T>
void list<T>::clear() {
    std::unique_lock lock(m_mutex);

    if (m_length > 0) {
        m_sentinel->prev->next = nullptr; // break cycle to prevent double free
        delete m_sentinel->next;

        m_sentinel->next = m_sentinel;
        m_sentinel->prev = m_sentinel;
        m_length = 0;
    }

    assert( m_sentinel->next == m_sentinel );
    assert( m_sentinel->prev == m_sentinel );
    assert( m_length == 0 );
}

}

#endif // LIST_HPP_
