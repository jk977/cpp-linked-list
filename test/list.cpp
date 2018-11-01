#define BOOST_TEST_MODULE List Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list.hpp"

BOOST_AUTO_TEST_CASE(length) {
    list<int> l;

    for (unsigned int i = 1; i <= 1000; i++) {
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

BOOST_AUTO_TEST_CASE(push) {
    list<int> l;

    l.push_front(1);
    BOOST_TEST( l.get_front().value_or(0) == 1 );

    l.push_back(2);
    BOOST_TEST( l.get_front().value_or(0) == 1 );

    l.push_front(3);
    BOOST_TEST( l.get_front().value_or(0) == 3 );

    std::size_t length = l.length();

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
        BOOST_TEST( *l.get(i + length) == i );
    }
}

BOOST_AUTO_TEST_CASE(foreach) {
    list<int> l;

    for (int i = 0; i < 100; i++) {
        l.push_back(i);
    }

    int index = 0;

    for (auto n : l) {
        BOOST_TEST( index == n );
        index++;
    }
}
