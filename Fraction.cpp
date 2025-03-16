#include <string>
#include <sstream>
#include "Fraction.h"

using namespace std;

int64_t gcd(int64_t a,int64_t b) {
    while (b) {
		int64_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void fraction_simplify(Fraction * f) {
	int64_t common = gcd(abs(f->numerator), abs(f->denominator));
    f->numerator /= common;
    f->denominator /= common;

	if(f->numerator < 0 && f->denominator < 0)
	{
		f->numerator = -f->numerator;
		f->denominator = -f->denominator;
	}
}

Fraction fraction_add(Fraction a, Fraction b) {
    Fraction result;

    result.numerator = a.numerator * b.denominator + b.numerator * a.denominator,
    result.denominator = a.denominator * b.denominator;

    fraction_simplify(&result);

    return result;
}

Fraction fraction_subtract(Fraction a, Fraction b) {
    Fraction result;

    result.numerator = a.numerator * b.denominator - b.numerator * a.denominator,
    result.denominator = a.denominator * b.denominator;

    fraction_simplify(&result);

    return result;
}

Fraction fraction_multiply(Fraction a, Fraction b) {
    Fraction result;

    result.numerator = a.numerator * b.numerator,
    result.denominator = a.denominator * b.denominator;

    fraction_simplify(&result);

    return result;
}

Fraction fraction_divide(Fraction a, Fraction b) {
    Fraction result;

    result.numerator = a.numerator * b.denominator;
    result.denominator = a.denominator * b.numerator;

    fraction_simplify(&result);
    return result;
}

string fraction_toString(Fraction f) {
    fraction_simplify(&f); 

    stringstream ss;

    if (f.denominator == 1) 
    {
        ss << f.numerator;
    }
    else if (f.numerator < f.denominator) 
    {
        ss << f.numerator << "/" << f.denominator;
    }
    else 
    {
        const int64_t integer = f.numerator / f.denominator;
        const int64_t remainder = f.numerator % f.denominator;

        ss << integer << "'" << remainder << "/" << f.denominator;
    }

    return ss.str();
}

int64_t fraction_compare(Fraction a, Fraction b)
{
    return a.numerator * b.denominator - b.numerator * a.denominator;
}