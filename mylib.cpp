#include <iostream>
#include <cfenv>
#include <cmath>
#include <limits>
#include "mylib.h"

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


