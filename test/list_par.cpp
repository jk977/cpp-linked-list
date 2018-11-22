#define BOOST_TEST_MODULE List Multithread Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list.hpp"

#include <thread>
#include <vector>

using dsa::list;

template<class Fn, class... T>
void execute_n_times(int n, Fn f, T... args) {
    std::vector<std::thread> threads(n);

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

    int constexpr times = 100;
    execute_n_times(times, push, std::ref(l));
    BOOST_TEST( l.length() == times * fill_size );

    std::vector<int> clone(l.length());

    for (int i = 0; i < l.length(); i++) {
        clone[i] = *l.get(i);
    }

    std::array<int, fill_size> count;
    count.fill(0);

    for (int i = 0; i < (int) clone.size(); i++) {
        int current = clone[i];
        count[current]++;
    }

    // check if all counts are equal
    for (auto n : count) {
        BOOST_TEST( count[0] == n );
    }

    // make sure all elements were counted
    BOOST_TEST( count[0] * (int) count.size() == l.length() );
}

BOOST_AUTO_TEST_CASE(pop) {
    list<int> l;
    int constexpr length = 10;
    auto popper = [](list<int>& l) {
        BOOST_TEST( l.pop_back().has_value() );
    };

    for (int i = 0; i < length; i++) {
        l.push_back(i);
    }

    BOOST_TEST( l.length() == length );

    execute_n_times(length, popper, std::ref(l));
    BOOST_TEST(l.empty());
}

BOOST_AUTO_TEST_CASE(map) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    auto add_all = [&l](int x) {
        l.map( [x](auto n) { return n+x; } );
    };

    int constexpr times = 100;
    execute_n_times(times, add_all, 1);

    for (int i = 0; i < l.length(); i++) {
        BOOST_TEST( *l.get(i) == (times + i) );
    }
}

BOOST_AUTO_TEST_CASE(modify) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    auto inc_all = [](list<int>& l) {
        for (int i = 0; i < l.length(); i++) {
            l.modify(i, [](auto n) {
                return n+1;
            });
        }
    };

    int constexpr times = 100;
    execute_n_times(times, inc_all, std::ref(l));

    for (int i = 0; i < l.length(); i++) {
        BOOST_TEST( *l.get(i) == (times + i) );
    }
}
