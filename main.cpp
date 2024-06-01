#include <iostream>
#include <boost/numeric/interval.hpp>
#include <limits>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;
using namespace boost::numeric;

// Define the interval type with higher precision (long double)
typedef interval<long double> Interval;

int main() {
    long double a = 1.0L;
    long double lower = std::nextafter(a,a-1);
    long double upper = std::nextafter(a, a+1);

    // Define an interval around 1.0 with minimal differences
    Interval i1(lower, upper);

    // Set precision for display
    cout << std::setprecision(21);

    // Print the interval
    cout << "Interval i1: [" << i1.lower() << ", " << i1.upper() << "]" << endl;
    cout << "Next representable value below 1.0: " << lower << endl;
    cout << "Next representable value above 1.0: " << upper << endl;

    return 0;
}
