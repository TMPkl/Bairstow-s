#include <iostream>
#include <cfenv>
#include <cmath>
#include <limits>

long double convertToNearestLower(long double num) {
    int originalRoundingMode = std::fegetround();

    std::fesetround(FE_DOWNWARD);

    long double lower = std::nextafter(num, -std::numeric_limits<long double>::infinity());

    if (num != lower) {
        num = lower;
    }
    std::fesetround(originalRoundingMode);

    return num;
}

int main() {
    long double num;
    std::cin >> num;
    long double lowerNum = convertToNearestLower(num);

    std::cout << std::fixed << std::setprecision(30);
    
    std::cout << "Original number: " << num << std::endl;
    std::cout << "Nearest lower or equal representable number: " << lowerNum << std::endl;

    return 0;
}
