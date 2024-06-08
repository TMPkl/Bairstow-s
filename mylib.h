#ifndef TEST_H
#define TEST_H
#include <boost/numeric/interval.hpp>
#include <vector>
using namespace std;
using namespace boost::numeric;

typedef interval<long double> Interval;

long double convertToNearestLower_(long double num);   // convert to nearest lower or stay the same if there is valid binary representatnion for the number
long double convertToNearestHigher_(long double num);   // convert to nearest higher or stay the same if there is valid binary representatnion for the number
vector<Interval> synthetic_division_I(const vector<Interval>& intervals, const Interval& u, const Interval& v) ;   // synthetic division on Interval objects
bool interval_includes_zero(const Interval& interval,long double precision);  // check if the interval includes zero
//inputs 
Interval input_interval2interval();   // create Interval object from input that is bigger or equal to the input, in case of situation where the input is not representable in binary
long double getPrecision();   // get the precision from the user
int getMaxIterations();   // get the max iterations from the user
void getInputParameters(long double& precision, int& max_iter);
vector<Interval> getInputPolyI2I();
//prints
void print_interval(const vector<Interval>& intervals);   // print the interval/s



void BarInterval2Interval(long double precision,int max_iter,vector<Interval> polynomial);   // main function, on Interval input 
void BarLongDouble(long double precision, int max_iter, vector<long double> polynomial);   // main function, on long double input
#endif