#include <iostream>
#include <vector>
#include <boost/numeric/interval.hpp>
#include <cfenv>
#include <cmath>
#include <limits>
#include "mylib.h"

using namespace boost::numeric;
typedef interval<long double> Interval;


long double convertToNearestLower_(long double num) {
    int originalRoundingMode = std::fegetround();

    std::fesetround(FE_DOWNWARD);

    long double lower = std::nextafter(num, -std::numeric_limits<long double>::infinity());

    if (num != lower) {
        num = lower;
    }
    std::fesetround(originalRoundingMode);

    return num;
}
long double convertToNearestHigher_(long double num) {
    int originalRoundingMode = std::fegetround();

    std::fesetround(FE_UPWARD);

    long double lower = std::nextafter(num, std::numeric_limits<long double>::infinity());

    if (num != lower) {
        num = lower;
    }
    std::fesetround(originalRoundingMode);

    return num;
}

Interval input_interval2interval(){
    long double lower, upper;
    std::cin >> lower >> upper;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Invalid input");
    }
    if (lower > upper) {
        throw std::invalid_argument("Invalid interval");
    }
    return Interval(convertToNearestLower_(lower), convertToNearestHigher_(upper));
}
