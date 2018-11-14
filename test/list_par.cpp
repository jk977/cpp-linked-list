#define BOOST_TEST_MODULE List Multithread Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list.hpp"

#include <thread>
#include <array>
#include <vector>

using dsa::list;

int constexpr thread_count = 100;
std::array<std::thread, thread_count> threads;

template<class Fn, class... T>
void execute_parallel(Fn f, T... args) {
    for (auto& t : threads) {
        t = std::thread(f, args...);
    }

    for (auto& t : threads) {
        t.join();
    }
}

BOOST_AUTO_TEST_CASE(push) {
    list<int> l;
    int constexpr fill_size = 100;

    auto push = [fill_size](list<int>& l) {
        for (int i = 0; i < fill_size; i++) {
            l.push_back(i);
        }
    };

    execute_parallel(push, std::ref(l));
    BOOST_TEST( l.length() == thread_count * fill_size );

    std::vector<int> clone(l.length());

    for (int i = 0; i < l.length(); i++) {
        clone[i] = *l.get(i);
    }

    std::sort(clone.begin(), clone.end());

    std::array<int, fill_size> count;
    count.fill(0);

    for (int i = 0; i < (int) clone.size(); i++) {
        int current = clone[i];
        count[current]++;
    }

    // check if all counts are equal
    for (auto& n : count) {
        BOOST_TEST( count[0] == n );
    }

    // make sure all elements were counted
    BOOST_TEST( count[0] * (int) count.size() == l.length() );
}

BOOST_AUTO_TEST_CASE(map) {
    list<int> l;
    bool has_run = false;
    auto runner = [&has_run](auto n) {
        has_run = true; // shouldn't run on an empty list
        return n;
    };

    l.map(runner);
    BOOST_TEST(!has_run);

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    auto add_all = [&l](int x) {
        l.map( [x](auto& n) { return n+x; } );
    };

    execute_parallel(add_all, 1);

    for (int i = 0; i < (int) l.length(); i++) {
        BOOST_TEST( *l.get(i) == (thread_count + i) );
    }
}

BOOST_AUTO_TEST_CASE(modify) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    auto inc_all = [](list<int>& l) {
        for (unsigned int i = 0; i < l.length(); i++) {
            l.modify(i, [](auto n) {
                return n+1;
            });
        }
    };

    execute_parallel(inc_all, std::ref(l));

    for (int i = 0; i < (int) l.length(); i++) {
        BOOST_TEST( *l.get(i) == (thread_count + i) );
    }
}
