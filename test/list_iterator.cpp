#define BOOST_TEST_MODULE List Iterator Test
#include <boost/test/included/unit_test.hpp>

#include "../src/list.hpp"

BOOST_AUTO_TEST_CASE(dereference) {
    struct test { int a; };

    list<test> l;

    test t1 = {0};
    test t2 = {5};
    test t3 = {6};

    l.push_back(t1);
    l.push_back(t2);
    l.push_back(t3);

    auto iter = l.begin();

    auto validate = [](auto it, int expected) {
        return (it->a == (*it).a)
            && (it->a == expected);
    };

    BOOST_TEST( validate(iter++, 0) );
    BOOST_TEST( validate(iter++, 5) );
    BOOST_TEST( validate(iter++, 6) );
}

BOOST_AUTO_TEST_CASE(arithmetic) {
    list<int> l;
    l.push_back(0);
    l.push_back(1);

    auto iter = l.begin();

    BOOST_TEST( *iter == 0 );
    BOOST_TEST( *++iter == 1 );

    BOOST_TEST( *iter == 1 );
    BOOST_TEST( *--iter == 0 );

    BOOST_TEST( *iter == 0 );
    BOOST_TEST( *iter++ == 0 );

    BOOST_TEST( *iter == 1 );
    BOOST_TEST( *iter-- == 1 );

    BOOST_TEST( *iter == 0 );
}
