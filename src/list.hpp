/*
 * Thread-safe linked list implementation.
 */

#ifndef TS_LIST_HPP_
#define TS_LIST_HPP_

#include <cstddef>
#include <optional>
#include <functional>

#include <mutex>
#include <shared_mutex>

#include "list_node.hpp"

namespace dsa::ts {

template<class T>
class list {
public:
    list();
    ~list();

    using modify_fn = std::function<T(T const&)>;
    using access_type = std::optional<T>;

    void push_front(T val);
    void push_back(T val);
    void insert(T val, std::size_t index);

    access_type pop_front();
    access_type pop_back();
    access_type pop(std::size_t index);

    access_type get_front() const;
    access_type get_back() const;
    access_type get(std::size_t index) const;

    void map(modify_fn const& f);

    void modify_front(modify_fn const& f);
    void modify_back(modify_fn const& f);
    void modify(std::size_t index, modify_fn const& f);

    std::size_t length() const;
    bool empty() const;
    void clear();

private:
    using node_t = detail::list_node<T>;

    void insert_empty(T val);
    void insert_middle(T val, std::size_t index);

    node_t*     node_at(std::size_t index) const;
    access_type pop_at(std::size_t index);
    void        modify_at(std::size_t index, modify_fn const& f);

    node_t* m_sentinel;
    std::shared_mutex mutable m_mutex;
    std::size_t m_length;
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
    // insert value into an empty list, assuming thread has exclusive ownership of list
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
void list<T>::insert_middle(T val, std::size_t index) {
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
typename list<T>::node_t* list<T>::node_at(std::size_t index) const {
    // gets node at specified index, assuming thread has ownership of list.
    auto current = m_sentinel->next;
    std::size_t i = 0;

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
typename list<T>::access_type list<T>::pop_at(std::size_t index) {
    // assumes thread has exclusive ownership of list and m_length > 0
    --m_length;
    return detail::pop_node(node_at(index));
}

template<class T>
typename list<T>::access_type list<T>::pop_front() {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        return std::nullopt;
    }

    return pop_at(0);
}

template<class T>
typename list<T>::access_type list<T>::pop_back() {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        return std::nullopt;
    }

    return pop_at(m_length-1);
}

template<class T>
typename list<T>::access_type list<T>::pop(std::size_t index) {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        return std::nullopt;
    }

    return pop_at(index);
}

template<class T>
typename list<T>::access_type list<T>::get_front() const {
    return get(0);
}

template<class T>
typename list<T>::access_type list<T>::get_back() const {
    std::shared_lock lock(m_mutex);
    return get(m_length-1);
}

template<class T>
typename list<T>::access_type list<T>::get(std::size_t index) const {
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
void list<T>::modify_at(std::size_t index, list<T>::modify_fn const& f) {
    // assumes thread has exclusive ownership of list.
    // to prevent data races while allowing mutation of specific elements,
    // elements are modified by providing an index as well as a function that
    // takes the element and returns the new value for the element.
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

    if (m_length == 0) {
        return;
    }

    modify_at(m_length-1, f);
}

template<class T>
void list<T>::modify(std::size_t index, list<T>::modify_fn const& f) {
    std::unique_lock lock(m_mutex);

    if (m_length == 0) {
        return;
    }

    modify_at(index, f);
}

template<class T>
std::size_t list<T>::length() const {
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
    // resets list to initial state (length() == 0)
    std::unique_lock lock(m_mutex);

    if (m_length > 0) {
        m_sentinel->prev->next = nullptr; // break cycle to prevent double free
        delete m_sentinel->next;

        m_sentinel->next = m_sentinel;
        m_sentinel->prev = m_sentinel;
        m_length = 0;
    }
}

}

#endif // TS_LIST_HPP_
