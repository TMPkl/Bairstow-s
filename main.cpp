#include <iostream>
#include <vector>
#include <boost/numeric/interval.hpp>
#include <limits>
#include <iomanip>
#include <cmath>
#include "main.h"

using namespace std;
using namespace boost::numeric;

typedef interval<long double> Interval;

vector<Interval> createIntervals(const vector<long double>& input_polynomial) {
    vector<Interval> intervals;
    for (const auto& val : input_polynomial) {
        long double lower = std::nextafter(val, -numeric_limits<long double>::infinity());
        long double upper = std::nextafter(val, numeric_limits<long double>::infinity());
        intervals.emplace_back(lower, upper);
    }
    return intervals;
}

vector<long double> input_single_interval_polynomial() {
    cout << "Enter the values of the coefficients starting from X^n to X^0. Enter any other character to stop." << endl;
    vector<long double> input_polynomial;
    while (true) {
        long double x;
        cout << "Enter a value for the polynomial (or any other character to stop): ";
        cin >> x;
        if (cin.fail()) {
            break;
        }
        input_polynomial.push_back(x);
    }
    return input_polynomial;
}

void print_interval(std::vector<long double> &input_polynomial, std::vector<Interval> &intervals)
{
    // Print the intervals
    for (size_t i = 0; i < input_polynomial.size(); ++i)
    {
        cout << " Interval: [" << intervals[i].lower() << ", " << intervals[i].upper() << "]" << endl;
    }
}

void print_interval_I(const vector<Interval>& intervals) {
    // Print the intervals
    for (const auto& interval : intervals) {
        cout << "Interval: [" << interval.lower() << ", " << interval.upper() << "]" << endl;
    }
}

vector<Interval> synthetic_divison_I(const vector<Interval>& intervals, const Interval& u, const Interval& v) {
    vector<Interval> results;
    for (int i = 0; i < intervals.size(); ++i) {
        if (i == 0){
            results.push_back(intervals[i]);
        }
        else if (i == 1)
        {
            results.push_back(intervals[i] + results[i-1]*u);
        }
        else
        {
            results.push_back(intervals[i] + results[i-1]*u + results[i-2]*v);
        }
    }
    return results;
}

Interval input_single_variable_I(const char variable) {
    long double u_n;
    cout << "Enter the value for " << variable << ": ";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (true) {
        if (!(cin >> u_n)) {
            cout << "Invalid input. Please enter a numerical value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return Interval(std::nextafter(u_n, -numeric_limits<long double>::infinity()), std::nextafter(u_n, numeric_limits<long double>::infinity()));
}



int main()
{
    vector<long double> input_polynomial = input_single_interval_polynomial();

    vector<Interval> input_polynomial_I = createIntervals(input_polynomial);

    vector<long double>().swap(input_polynomial);

    cout << std::setprecision(21);  // Set the precision to 21 decimal places

    print_interval_I(input_polynomial_I);


    Interval u = input_single_variable_I('u');
    Interval v = input_single_variable_I('v');

    cout << "u = [" << u.lower() << ", " << u.upper() << "]" << endl;
    cout << "v = [" << v.lower() << ", " << v.upper() << "]" << endl;

    vector<Interval> b = synthetic_divison_I(input_polynomial_I, u, v);

    print_interval_I(b);

    return 0;
}
