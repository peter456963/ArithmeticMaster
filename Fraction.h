#ifndef __FRACTION_H__
#define __FRACTION_H__

#include <string>

struct Fraction {
    int64_t numerator;   // 分子
	int64_t denominator; // 分母
};

int64_t gcd(int64_t a,int64_t b);
void fraction_simplify(Fraction* f);
Fraction fraction_add(Fraction a, Fraction b);
Fraction fraction_subtract(Fraction a, Fraction b);
Fraction fraction_multiply(Fraction a, Fraction b);
Fraction fraction_divide(Fraction a, Fraction b);
int64_t fraction_compare(Fraction a, Fraction b);
std::string fraction_toString(Fraction f);

#endif