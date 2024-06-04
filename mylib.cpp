#include <iostream>
#include <vector>
#include <boost/numeric/interval.hpp>
#include <cfenv>
#include <cmath>
#include <limits>
#include "mylib.h"

using namespace boost::numeric;
using namespace std;
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
vector<Interval> synthetic_division_I(const vector<Interval>& intervals, const Interval& u, const Interval& v) {
    vector<Interval> results;
    for (size_t i = 0; i < intervals.size(); ++i) {
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


bool interval_includes_zero(const Interval& interval,long double precision) {
    return interval.lower()-precision <= 0 && interval.upper()+precision >= 0;
}

///////////////inputs////////////////////////////////////////////////////////////////////////////////////////////////////////
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

long double getPrecision() {
    long double precision;
    cout << "Enter the precision: ";
    cin >> precision;
    if (precision == -1) {
        precision = 0;
    }
    return precision;
}

int getMaxIterations() {
    int max_iter;
    cout << "Enter the maximum number of iterations: ";
    cin >> max_iter;
    if (max_iter == -1) {
        max_iter = 10e+6;
    }
    return max_iter;
}

void getInputParameters(long double& precision, int& max_iter) {
    precision = getPrecision();
    max_iter = getMaxIterations();
}

vector<Interval> getInputPolyI2I() {
        std::cout << "Enter the intervals of polynomial coefficients starting from X^n to X^0. Enter any other character to stop. Make sure to write it in correct order!" << std::endl;
        Interval input;
        vector<Interval> polynomial;
        while (true) {
            try {
                input = input_interval2interval();
            } catch (const std::invalid_argument& e) {
                break;
            }
            polynomial.push_back(input);
        }
        return polynomial;
    }
////////////////////////////////////////////////////////////////////////////////////////////////////////

//////prints////////////////////////////////////////////////////////////////////////////////////////////////////////
void print_interval(const vector<Interval>& intervals) {
    for (const auto& interval : intervals) {
        cout <<"[" << interval.lower() << ", " << interval.upper() << "]" << endl<<scientific;
    }}
////////////////////////////////////////////////////////////////////////////////////////////////////////    


void BarInterval2Interval(long double precision,int max_iter,vector<Interval> polynomial){
    

    Interval u = interval<long double>(2.0, 2.0);
    Interval v = interval<long double>(-10.0, -10.0);

    vector<Interval> b;
    vector<Interval> c;

    int max_iter_in = max_iter;

    while(max_iter--){
        size_t degree = polynomial.size() - 1;
        if (degree == 0) {
            break;
        } else if (degree == 1){
            print_interval({polynomial[0]});
            break;
        } else if (degree == 2){
            long double a = polynomial[0].upper();
            long double b = polynomial[1].lower(); // sprawdzam czy pierwiastek jest rzeczywisty dla skrajnego przypadku delty (b małe a i c duże)
            long double c_ = polynomial[2].upper();
            long double delta = b * b - 4 * a * c_;

            if (delta > 0) {
                Interval sqrt_delta = sqrt(Interval(delta));
                Interval root1 = (-polynomial[1] - sqrt_delta) / (Interval(2.0) * polynomial[0]);
                Interval root2 = (-polynomial[1] + sqrt_delta) / (Interval(2.0) * polynomial[0]);
                //cout << "The polynomial is of degree 2. The 2 distinct roots: ";
                print_interval({root1});
                print_interval({root2});
            } else if (delta == 0) {
                Interval root = -polynomial[1] / (Interval(2.0) * polynomial[0]);
                //cout << "The polynomial is of degree 2. The 2 same roots: ";
                print_interval({root});
                print_interval({root});
            } else {
                Interval root1real = -polynomial[1] / (Interval(2.0) * polynomial[0]);
                Interval root1imaginary = sqrt(Interval(-delta)) / (Interval(2.0) * polynomial[0]);
                Interval root2real = root1real;
                Interval root2imaginary = -root1imaginary;
                //cout << "The polynomial is of degree 2. The 2 complex roots: ";
                cout << "[" << root1real.lower() << ", " << root1real.upper() << "] + [" << root1imaginary.lower() << ", " << root1imaginary.upper() << "]i" << endl;
                cout << "[" <<root2real.lower() << ", " << root2real.upper() << "] + [" << root2imaginary.lower() << ", " << root2imaginary.upper() << "]i" << endl;
            }
            break;
        }else {
            b = synthetic_division_I(polynomial, u, v);
     
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
                    polynomial = vector<Interval>(b.begin(), b.end() - 2);

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
                    polynomial = vector<Interval>(b.begin(), b.end() - 2);

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

