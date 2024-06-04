#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath> // Include cmath for std::nextafter

using namespace std;

long double lower_bounder(long double original) {
    string str = to_string(original);
    stringstream ss;
    ss << std::setprecision(21) << str;
    long double converted;
    ss >> converted;

    if (original == converted) {
        return original;
    } else {
        return std::nextafter(original, -numeric_limits<long double>::infinity());
    }
}

int main() {
    long double x;
    cout << setprecision(21)<<scientific;
    cin >> x;
    cout << lower_bounder(x) << endl;
    return 0;
}
