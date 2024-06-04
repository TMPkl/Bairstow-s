#ifndef TEST_H
#define TEST_H
#include <boost/numeric/interval.hpp>
using namespace boost::numeric;
typedef interval<long double> Interval;

long double convertToNearestLower_(long double num);   // convert to nearest lower or stay the same if there is valid binary representatnion for the number
long double convertToNearestHigher_(long double num);   // convert to nearest higher or stay the same if there is valid binary representatnion for the number
 
Interval input_interval2interval();   // create Interval object from input that is bigger or equal to the input, in case of situation where the input is not representable in binary
#endif