#define BOOST_TEST_MODULE List Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list.hpp"
#include <functional>

using dsa::list;

BOOST_AUTO_TEST_CASE(push) {
    list<int> l;

    l.push_front(1);
    BOOST_TEST( *l.get_front() == 1 );

    l.push_back(2);
    BOOST_TEST( *l.get_front() == 1 );

    l.push_front(3);
    BOOST_TEST( *l.get_front() == 3 );

    auto length = l.length();

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
        BOOST_TEST( *l.get(i + length) == i );
    }
}

BOOST_AUTO_TEST_CASE(insert) {
    list<int> l;

    l.insert(0, -1);
    BOOST_TEST( l.empty() );

    l.insert(10, 0);
    BOOST_TEST( *l.get(0) == 10 );

    l.insert(11, 1);
    BOOST_TEST( *l.get_back() == 11 );

    l.insert(3, 2);
    BOOST_TEST( *l.get(2) == 3 );

    l.insert(6, 2);
    BOOST_TEST( *l.get(2) == 6 );
    BOOST_TEST( *l.get(3) == 3 );
}

BOOST_AUTO_TEST_CASE(pop) {
    list<int> l;

    BOOST_TEST( !l.pop_front().has_value() );
    BOOST_TEST( !l.pop_back().has_value() );

    BOOST_TEST( !l.pop(-1).has_value() );
    BOOST_TEST( !l.pop(1).has_value() );
    BOOST_TEST( !l.pop(10).has_value() );

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
    BOOST_TEST( l.length() == 2 );
}

BOOST_AUTO_TEST_CASE(get) {
    list<int> l;
    auto small_value = 1 << (sizeof(1) * 8 - 1);

    BOOST_TEST( !l.get(10).has_value() );
    BOOST_TEST( !l.get(-1).has_value() );
    BOOST_TEST( !l.get(small_value).has_value() );

    BOOST_TEST( !l.get_front().has_value() );
    BOOST_TEST( !l.get_back().has_value() );

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
        BOOST_TEST( *l.get(i) == i );
    }

    BOOST_TEST( *l.get_front() == 0 );
    BOOST_TEST( *l.get_back() == 99 );
    BOOST_TEST( !l.get(999).has_value() );
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

    l.map( [](auto n) { return n+1; } );

    for (int i = 0; i < (int) l.length(); i++) {
        BOOST_TEST( *l.get(i) == (i+1) );
    }
}

BOOST_AUTO_TEST_CASE(modify) {
    list<int> l;
    bool has_run = false;
    auto runner = [&has_run](auto n) {
        has_run = true;
        return n;
    };

    BOOST_TEST( l.empty() );

    l.modify_front(runner);
    BOOST_TEST(!has_run);

    l.modify_back(runner);
    BOOST_TEST(!has_run);

    for (int i = 0; i < 5; i++) {
        l.push_back(i+1);
    }

    l.modify(-1, runner);
    BOOST_TEST(!has_run);

    l.modify_front( [](auto n) { return n*2; } );
    BOOST_TEST( *l.get_front() == 2 );

    l.modify_back( [](auto n) { return n+3; } );
    BOOST_TEST( *l.get_back() == 8 );

    l.modify(2, [](auto n) { return n+1; } );
    BOOST_TEST( *l.get(2) == 4 );
}

BOOST_AUTO_TEST_CASE(length) {
    list<int> l;
    BOOST_TEST( l.length() == 0 );

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
    BOOST_TEST( l.length() == 0 );

    l.clear();
    BOOST_TEST( l.length() == 0 );

    l.push_back(0);
    BOOST_TEST( l.length() == 1 );

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    BOOST_TEST( l.length() == 101 );

    l.clear();
    BOOST_TEST( l.length() == 0 );
    BOOST_TEST(l.empty());
}