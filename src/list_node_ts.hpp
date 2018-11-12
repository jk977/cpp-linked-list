#ifndef LIST_NODE_TS_HPP_
#define LIST_NODE_TS_HPP_

#include "list_node.hpp"
#include <mutex>

namespace dsa::detail {

template<class T>
struct list_node_ts: public list_node<T> {
    std::mutex mtx;
};

}

#endif // LIST_NODE_TS_HPP_
