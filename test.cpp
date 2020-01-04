#include "square.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE testFigures

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(testArea) {
    vertex<double> p1{1, 0};
    vertex<double> p2{1, 1};
    vertex<double> p3{0, 1};
    vertex<double> p4{0, 0};
    square<double> s(p1,p2,p3,p4);
    BOOST_CHECK_EQUAL(s.area(), 1);
}

BOOST_AUTO_TEST_CASE(testCenter) {
    vertex<double> p1{0, 0};
    vertex<double> p2{1, 0};
    vertex<double> p3{1, 1};
    vertex<double> p4{0, 1};
    square<double> s(p1,p2,p3,p4);
    BOOST_CHECK_EQUAL(s.center().x, 0.5);
    BOOST_CHECK_EQUAL(s.center().y, 0.5);   
}
