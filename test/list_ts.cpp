#define BOOST_TEST_MODULE List Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list_ts.hpp"

#include <functional>
#include <thread>
#include <vector>

template<class T>
using list = dsa::list_ts<T>;

BOOST_AUTO_TEST_CASE(multithreading) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    auto length = [](auto& l) { return (int) l.length(); };

    auto process_list = [&length](list<int>& l) {
        for (int i = 0; i < length(l); i++) {
            ++l[i];
        }
    };

    int const thread_count = 500;
    std::vector<std::thread> threads(thread_count);

    for (int i = 0; i < thread_count; i++) {
        threads[i] = std::thread(process_list, std::ref(l));
    }

    for (int i = 0; i < thread_count; i++) {
        threads[i].join();
    }

    for (int i = 0; i < length(l); i++) {
        BOOST_TEST( l[i] == (thread_count + i) );
    }
}

