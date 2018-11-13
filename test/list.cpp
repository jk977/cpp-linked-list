#define BOOST_TEST_MODULE List Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list.hpp"

#include <iostream>
#include <functional>
#include <thread>
#include <array>

using dsa::ts::list;

int constexpr thread_count = 500;
std::array<std::thread, thread_count> threads;

BOOST_AUTO_TEST_CASE(push) {
    list<int> l;

    l.push_front(1);
    BOOST_TEST( *l.get_front() == 1 );

    l.push_back(2);
    BOOST_TEST( *l.get_front() == 1 );

    l.push_front(3);
    BOOST_TEST( *l.get_front() == 3 );

    std::size_t length = l.length();

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
        BOOST_TEST( *l.get(i + length) == i );
    }
}

BOOST_AUTO_TEST_CASE(insert) {
    list<int> l;

    for (int i = 0; i < 10; i++) {
        l.push_front(i);
    }

    l.insert(10, 0);
    BOOST_TEST( *l.get(0) == 10 );

    l.insert(11, l.length());
    BOOST_TEST( *l.get_back() == 11 );

    l.insert(3, 3);
    BOOST_TEST( *l.get(3) == 3 );

    l.insert(6, 8);
    BOOST_TEST( *l.get(8) == 6 );
}

BOOST_AUTO_TEST_CASE(pop) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
        l.push_front(-i);

        BOOST_TEST( *l.pop_back() == i );
        BOOST_TEST( *l.pop_front() == -i );
    }

    l.push_front(4);
    l.push_front(3);
    l.push_front(2);
    l.push_front(1);
    l.push_front(0);

    BOOST_TEST( *l.pop(2) == 2 );
    BOOST_TEST( *l.pop_front() == 0 );
    BOOST_TEST( *l.pop_back() == 4 );
    BOOST_TEST( (int) l.length() == 2 );
}

BOOST_AUTO_TEST_CASE(get) {
    list<int> l;

    BOOST_TEST( !l.get(10).has_value() );
    BOOST_TEST( !l.get_front().has_value() );
    BOOST_TEST( !l.get_back().has_value() );

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
        BOOST_TEST( *l.get(i) == i );
        BOOST_TEST( *l.get(i) == i );
    }

    BOOST_TEST( *l.get_front() == 0 );
    BOOST_TEST( *l.get_back() == 99 );
    BOOST_TEST( !l.get(999).has_value() );
}

BOOST_AUTO_TEST_CASE(map) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    auto add_all = [&l](int x) {
        l.map( [x](auto& n) { return n+x; } );
    };

    for (int i = 0; i < thread_count; i++) {
        threads[i] = std::thread(add_all, 1);
    }

    for (int i = 0; i < thread_count; i++) {
        threads[i].join();
    }

    for (int i = 0; i < (int) l.length(); i++) {
        BOOST_TEST( *l.get(i) == (thread_count + i) );
    }
}

BOOST_AUTO_TEST_CASE(modify) {
    list<int> l;

    for (int i = 0; i < 5; i++) {
        l.push_back(i+1);
    }

    l.modify_front( [](auto n) { return n*2; } );
    BOOST_TEST( *l.get_front() == 2 );

    l.modify_back( [](auto n) { return n+3; } );
    BOOST_TEST( *l.get_back() == 8 );

    l.modify(2, [](auto n) { return n+1; } );
    BOOST_TEST( *l.get(2) == 4 );
}

BOOST_AUTO_TEST_CASE(length) {
    list<int> l;

    for (unsigned int i = 1; i <= 100; i++) {
        l.push_back(i);
        BOOST_TEST( l.length() == i );
    }
}

BOOST_AUTO_TEST_CASE(empty) {
    list<int> l;

    BOOST_TEST( l.empty() );

    l.push_back(1);
    BOOST_TEST( !l.empty() );

    l.push_back(2);
    BOOST_TEST( !l.empty() );
}

BOOST_AUTO_TEST_CASE(clear) {
    list<int> l;

    // to satisfy the compiler with signed/unsigned comparisons
    auto length = [](auto& l) { return (int) l.length(); };

    BOOST_TEST( length(l) == 0 );

    l.clear();
    BOOST_TEST( length(l) == 0 );

    l.push_back(0);
    BOOST_TEST( length(l) == 1 );

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    BOOST_TEST( length(l) == 101 );

    l.clear();
    BOOST_TEST( length(l) == 0 );
}

BOOST_AUTO_TEST_CASE(threaded_write) {
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

    for (int i = 0; i < thread_count; i++) {
        threads[i] = std::thread(inc_all, std::ref(l));
    }

    for (int i = 0; i < thread_count; i++) {
        threads[i].join();
    }

    for (int i = 0; i < (int) l.length(); i++) {
        BOOST_TEST( *l.get(i) == (thread_count + i) );
    }
}
