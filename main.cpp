#include <iostream>
#include <vector>
#include <boost/numeric/interval.hpp>
#include <limits>
#include <iomanip>
#include <cmath>
#include "mylib.h"
using namespace std;
using namespace boost::numeric;

typedef interval<long double> Interval;

#define COUT_PRECISION 21

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
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        input_polynomial.push_back(x);
    }
    return input_polynomial;
}

// void print_interval(const vector<Interval>& intervals) {
//     for (const auto& interval : intervals) {
//         cout <<"[" << interval.lower() << ", " << interval.upper() << "]" << endl<<scientific;
//     }
// } in mylib files 

// vector<Interval> synthetic_division_I(const vector<Interval>& intervals, const Interval& u, const Interval& v) {
//     vector<Interval> results;
//     for (size_t i = 0; i < intervals.size(); ++i) {
//         if (i == 0) {
//             results.push_back(intervals[i]);
//         } else if (i == 1) {
//             results.push_back(intervals[i] + results[i-1] * u);
//         } else {
//             results.push_back(intervals[i] + results[i-1] * u + results[i-2] * v);
//         }
//     }
//     return results;
// } in mylib files

Interval input_single_variable_I(const char variable) {
    long double u_n;
    cout << "Enter the value for " << variable << ": ";
    while (true) {
        cin >> u_n;
        if (cin.fail()) {
            cout << "Invalid input. Please enter a numerical value." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            break;
        }
    }
    return Interval(std::nextafter(u_n, -numeric_limits<long double>::infinity()), std::nextafter(u_n, numeric_limits<long double>::infinity()));
}

// bool interval_includes_zero(const Interval& interval,long double precision) {
//     return interval.lower()-precision <= 0 && interval.upper()+precision >= 0;
// }

void BarSInterval() {
    vector<long double> input_polynomial = input_single_interval_polynomial();
    vector<Interval> input_polynomial_I = createIntervals(input_polynomial);



    Interval u = interval<long double>(2.0, 2.0);
    Interval v = interval<long double>(-10.0, -10.0);

    vector<Interval> b;
    vector<Interval> c;

    int max_iter = getMaxIterations();
    int max_iter_in = max_iter;
    long double precision = getPrecision();


    while (true && max_iter--) {
        size_t degree = input_polynomial_I.size() - 1;
        if (degree == 0) {
            //cout << "The polynomial is of degree 0." << endl;
            break;
        } else if (degree == 1) {
            print_interval({input_polynomial_I[0]});
            break;
        } else if (degree == 2) {
            long double a = input_polynomial_I[0].upper();
            long double b = input_polynomial_I[1].lower(); // sprawdzam czy pierwiastek jest rzeczywisty dla skrajnego przypadku delty (b małe a i c duże)
            long double c_ = input_polynomial_I[2].upper();
            long double delta = b * b - 4 * a * c_;

            if (delta > 0) {
                Interval sqrt_delta = sqrt(Interval(delta));
                Interval root1 = (-input_polynomial_I[1] - sqrt_delta) / (Interval(2.0) * input_polynomial_I[0]);
                Interval root2 = (-input_polynomial_I[1] + sqrt_delta) / (Interval(2.0) * input_polynomial_I[0]);
                //cout << "The polynomial is of degree 2. The 2 distinct roots: ";
                print_interval({root1});
                print_interval({root2});
            } else if (delta == 0) {
                Interval root = -input_polynomial_I[1] / (Interval(2.0) * input_polynomial_I[0]);
                //cout << "The polynomial is of degree 2. The 2 same roots: ";
                print_interval({root});
                print_interval({root});
            } else {
                Interval root1real = -input_polynomial_I[1] / (Interval(2.0) * input_polynomial_I[0]);
                Interval root1imaginary = sqrt(Interval(-delta)) / (Interval(2.0) * input_polynomial_I[0]);
                Interval root2real = root1real;
                Interval root2imaginary = -root1imaginary;
                //cout << "The polynomial is of degree 2. The 2 complex roots: ";
                cout << root1real.lower() << ", " << root1real.upper() << "] + [" << root1imaginary.lower() << ", " << root1imaginary.upper() << "]i" << endl;
                cout << root2real.lower() << ", " << root2real.upper() << "] + [" << root2imaginary.lower() << ", " << root2imaginary.upper() << "]i" << endl;
            }
            break;
        } else {
            b = synthetic_division_I(input_polynomial_I, u, v);
     
            if (interval_includes_zero(b[b.size()-1],precision) && interval_includes_zero(b[b.size()-2],precision)) {
                //cout << "Last 2 coefficients are zero." << endl;
                max_iter = max_iter_in;
                if (u.lower()*u.lower() + 4*v.lower() < 0)
                {
                    Interval root1real = -u / (Interval(2.0,2.0));
                    Interval root1imaginary = sqrt(-u*u - Interval(4.0,4.0)*v) / (Interval(2.0,2.0));
                    Interval root2real = root1real;
                    Interval root2imaginary = -root1imaginary;
                    cout << root1real.lower() << ", " << root1real.upper() << "] + [" << root1imaginary.lower() << ", " << root1imaginary.upper() << "]i" << endl;
                    cout <<  root2real.lower() << ", " << root2real.upper() << "] + [" << root2imaginary.lower() << ", " << root2imaginary.upper() << "]i" << endl;
                    input_polynomial_I = vector<Interval>(b.begin(), b.end() - 2);

                }
                else 
                {
                    //cout << "2 real roots: ";
                    Interval root1 = (u + sqrt(u*u + Interval(4,4)*v ))*Interval(0.5,0.5);
                    Interval root2 = (u - sqrt(u*u + Interval(4,4)*v ))*Interval(0.5,0.5);
                    //cout << "The 2 roots are: \n";
                    //cout << "root 1:";
                    print_interval({root1});
                    //cout << "root 2:";
                    print_interval({root2});
                    input_polynomial_I = vector<Interval>(b.begin(), b.end() - 2);

                }
            } else {

                c = synthetic_division_I(b, u, v);
                
                long double del_u_low = (b[b.size()-1].lower()*c[c.size()-4].lower() - b[b.size()-2].lower()*c[c.size()-3].lower())/(c[c.size()-3].lower()*c[c.size()-3].lower()-c[c.size()-2].lower()*c[c.size()-4].lower());
                long double del_u_up = (b[b.size()-1].upper()*c[c.size()-4].upper() - b[b.size()-2].upper()*c[c.size()-3].upper())/(c[c.size()-3].upper()*c[c.size()-3].upper()-c[c.size()-2].upper()*c[c.size()-4].upper());

                long double del_v_low = (b[b.size()-2].lower()*c[c.size()-2].lower() - b[b.size()-1].lower()*c[c.size()-3].lower())/(c[c.size()-3].lower()*c[c.size()-3].lower()-c[c.size()-2].lower()*c[c.size()-4].lower());                 
                long double del_v_up = (b[b.size()-2].upper()*c[c.size()-2].upper() - b[b.size()-1].upper()*c[c.size()-3].upper())/(c[c.size()-3].upper()*c[c.size()-3].upper()-c[c.size()-2].upper()*c[c.size()-4].upper());

                Interval del_u = Interval(min(del_u_low, del_u_up), max(del_u_low, del_u_up));
                Interval del_v = Interval(min(del_v_low, del_v_up), max(del_v_low, del_v_up));
                
                u += del_u;
                v += del_v;
            }
        }
    }
}



int main(){
    cout << std::setprecision(COUT_PRECISION) << scientific;  // Set the precision to 21 decimal places

    //BarSInterval();

    // long double x;
    // cin >> x;
    // long double x_l = convertToNearestLower_(x);
    // long double x_h = convertToNearestHigher_(x);

    // cout << x <<" "<<x_l << " " <<x_h<< endl;
    // interval test = input_interval2interval();
    // cout << test.lower() << " " << test.upper() << endl;
    BarInterval2Interval();
    return 0;
}
