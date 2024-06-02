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

void print_interval(const vector<Interval>& intervals) {
    for (const auto& interval : intervals) {
        cout << "Interval: [" << interval.lower() << ", " << interval.upper() << "]" << endl;
    }
}

vector<Interval> synthetic_divison_I(const vector<Interval>& intervals, const Interval& u, const Interval& v) {
    vector<Interval> results;
    for (int i = 0; i < intervals.size(); ++i) {
        if (i == 0) { 
            results.push_back(intervals[i]);
        } else if (i == 1) {
            results.push_back(intervals[i] + results[i-1] * u);
        } else {
            results.push_back(intervals[i] + results[i-1] * u + results[i-2] * v);
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

bool interval_includes_zero(const Interval& interval) {
    return interval.lower() <= 0 && interval.upper() >= 0;
}

int main() {
    vector<long double> input_polynomial = input_single_interval_polynomial();
    vector<Interval> input_polynomial_I = createIntervals(input_polynomial);
    vector<long double>().swap(input_polynomial);

    cout << std::setprecision(21);  // Set the precision to 21 decimal places
    print_interval(input_polynomial_I);

    Interval u = input_single_variable_I('u');
    Interval v = input_single_variable_I('v');

    cout << "u = [" << u.lower() << ", " << u.upper() << "]" << endl;
    cout << "v = [" << v.lower() << ", " << v.upper() << "]" << endl;

    while (true) {
        if (input_polynomial_I.size() == 1) {
            cout << "The polynomial is of degree 0." << endl;
            break;
        } else if (input_polynomial_I.size() == 2) {
            cout << "The polynomial is of degree 1. The root is: " << -input_polynomial_I[1].lower() / input_polynomial_I[0].lower() << endl;
            break;
        } else if (input_polynomial_I.size() == 3) {
            long double a = input_polynomial_I[0].lower();
            long double b = input_polynomial_I[1].lower();
            long double c = input_polynomial_I[2].lower();
            long double delta_lower = b * b - 4 * a * c;
            long double delta_upper = input_polynomial_I[1].upper() * input_polynomial_I[1].upper() - 4 * input_polynomial_I[0].upper() * input_polynomial_I[2].upper();
            Interval delta = Interval(delta_lower, delta_upper);
            if (interval_includes_zero(delta)) {
                Interval root = -input_polynomial_I[1] / (Interval(2.0) * input_polynomial_I[0]);
                cout << "The polynomial is of degree 2. The 2 same roots: ";
                print_interval({root});
            } else if (delta.lower() > 0) {
                Interval sqrt_delta = sqrt(delta);
                Interval root1 = (-input_polynomial_I[1] - sqrt_delta) / (Interval(2.0) * input_polynomial_I[0]);
                Interval root2 = (-input_polynomial_I[1] + sqrt_delta) / (Interval(2.0) * input_polynomial_I[0]);
                cout << "The polynomial is of degree 2. The 2 distinct roots: ";
                print_interval({root1});
                print_interval({root2});
            } else {
                cout << "The polynomial is of degree 2. The 2 complex roots: ";
                Interval root1real = -input_polynomial_I[1] / (Interval(2.0) * input_polynomial_I[0]);
                Interval root1imaginary = sqrt(-delta) / (Interval(2.0) * input_polynomial_I[0]);
                Interval root2real = root1real;
                Interval root2imaginary = -root1imaginary;
                cout << "Root 1: [" << root1real.lower() << ", " << root1real.upper() << "] + [" << root1imaginary.lower() << ", " << root1imaginary.upper() << "]i" << endl;
                cout << "Root 2: [" << root2real.lower() << ", " << root2real.upper() << "] + [" << root2imaginary.lower() << ", " << root2imaginary.upper() << "]i" << endl;
            }
            break;
        } else {
            vector<Interval> b = synthetic_divison_I(input_polynomial_I, u, v);
            print_interval(b);

            if (interval_includes_zero(b[b.size()-1]) && interval_includes_zero(b[b.size()-2])) {
                cout << "Last 2 coefficients are zero." << endl;
                break;
            } else {
                input_polynomial_I = b;
            }
        }
    }

    return 0;
}
