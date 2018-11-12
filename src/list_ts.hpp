#ifndef LIST_TS_HPP_
#define LIST_TS_HPP_

#include "list_node_ts.hpp"
#include "list.hpp"

namespace dsa {

template<class T>
class list_ts: public list<T> {
public:
    using iterator = void;
    using reverse_iterator = void;

    iterator begin() = delete;
    iterator end() = delete;

    reverse_iterator rbegin() = delete;
    reverse_iterator rend() = delete;

private:
    using node_t = detail::list_node_ts<T>;
};

}

#endif // LIST_TS_HPP_
