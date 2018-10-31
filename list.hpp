#ifndef LIST_HPP_
#define LIST_HPP_

#include <cstdlib>
#include <memory>

#include "list_node.hpp"

template<class T>
class list {
public:
    list();
    ~list();

    void push_front(T val);
    void push_back(T val);

    T get_front();
    T get_last();
    T get(std::size_t index);

    bool empty();
    std::size_t length();

private:
    std::size_t m_length;
    std::unique_ptr< list_node<T> > m_sentinel;
};

template<class T>
list<T>::list(): m_length(0) {
    m_sentinel = make_unique(T());
}

template<class T>
list<T>::~list() {
    auto& tail = m_sentinel->prev;

    if (tail == nullptr) {
        // sever tail from sentinel to prevent double free
        tail->next = nullptr;
    }

    delete m_sentinel;
}

#endif // LIST_HPP_
