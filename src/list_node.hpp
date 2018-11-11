#ifndef LIST_NODE_HPP_
#define LIST_NODE_HPP_

namespace dsa {

template<class T>
struct list_node {
    list_node() = default;
    list_node(T val): value(val) {}
    ~list_node() { delete next; }

    T value;
    list_node<T>* prev;
    list_node<T>* next;
};

template<class T>
std::optional<T> value_of(list_node<T>* node) {
    return (node != nullptr) ?
        std::make_optional(node->value) :
        std::nullopt;
}

template<class T>
std::optional<T> pop_node(list_node<T>* node) {
    // remove given node from surrounding nodes and return its value

    if (node == nullptr) {
        return std::nullopt;
    }

    // if node isn't null, it's guaranteed to have connections due to sentinel
    auto node_before = node->prev;
    auto node_after = node->next;
    auto value = value_of(node);

    // isolate target node and rewire surrounding nodes
    node->next = nullptr;
    node_before->next = node_after;
    node_after->prev = node_before;
    delete node;

    return value;
}

}

#endif // LIST_NODE_HPP_
