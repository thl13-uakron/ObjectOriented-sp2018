/*
Date: 1 March 2018
Author: Thomas Li (thl13)
Purpose: Create a value-semantic class that simulates a rational number
*/

#include <iostream>
#include <istream>
#include <ostream>

class Rational
{
private:
    // rational numbers are expressed as the numerator divided by the denomiator
    // the numerator can be any int, the denominator can be any int except 0
    int numerator;
    int denominator;
public:
    // constructors
    Rational() = default; // default constructor assigns default value to member data
    Rational(int n, int d) : numerator(n), denominator(d) // constructor for assigning numerator and denominator values at initialization
    {
        if (d == 0) throw std::logic_error("Error: denominator cannot equal zero"); // CONTRACT: denominator cannot equal zero
    }

    // assignment operator
    void operator=(const Rational& r)
    {
        numerator = r.numerator;
        denominator = r.denominator;
    }

    // arithmetic operators
    Rational operator*(Rational& r) const {return *(new Rational(numerator * r.numerator, denominator * r.denominator));} // (a/b)*(c/d) = ab/cd
    Rational operator/(Rational& r) const {return *(new Rational(numerator * r.denominator, denominator * r.numerator));} // (a/b)/(c/d) = (a/b)*(d/c) = ad/bc
    Rational operator+(Rational& r) const {return *(new Rational((numerator * r.denominator) + (r.numerator * denominator), denominator * r.denominator));} // (a/b)+(c/d) = (ad+cb)/(bd)
    Rational operator-(Rational& r) const {return *(new Rational((numerator * r.denominator) - (r.numerator * denominator), denominator * r.denominator));} // (a/b)-(c/d) = (ad-cb)/(bd)

    // arithmetic change operators
    void operator*=(Rational& r) {*this = (*this * r);} // shortcuts
    void operator/=(Rational& r) {*this = (*this / r);}
    void operator+=(Rational& r) {*this = (*this + r);}
    void operator-=(Rational& r) {*this = (*this - r);}

    // integer operators
    bool operator==(int i) const {return numerator == i * denominator;}
    bool operator>(int i) const {return numerator > i * denominator;}
    bool operator<(int i) const {return numerator < i * denominator;}
    bool operator!=(int i) const {return !(*this == i);}
    bool operator<=(int i) const {return !(*this > i);}
    bool operator>=(int i) const {return !(*this < i);}

    // equality operators
    bool operator==(Rational& r) const {return *this / r == 1;} // (a/b == c/d) if ((a/b)/(c/d) == 1)
    bool operator!=(Rational& r) const {return !(*this == r);} // shortcut

    // comparison operators
    bool operator>(Rational& r) const {return *this / r > 1;} // same logic as applied to the equality operator
    bool operator<(Rational& r) const {return *this / r < 1;}
    bool operator>=(Rational& r) const {return !(*this < r);} // shortcut
    bool operator<=(Rational& r) const {return !(*this > r);} // shortcut

    // input/output operators
    std::istream operator>>(std::istream i);
    std::ostream operator<<(std::ostream o);
};



int main()
{
    return 0;
}
