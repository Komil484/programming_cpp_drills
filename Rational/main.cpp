#include <iostream>
#include <algorithm>

using namespace std;

class Rational {
public:

    class Invalid : runtime_error {
        friend class Rational;
        Invalid() : runtime_error("Invalid Rational") {}
        Invalid(string what_arg) : runtime_error(what_arg) {}
    };

    Rational();
    Rational(int numerator);
    Rational(int numerator, int denominator);

    int numerator() const;
    int denominator() const;
    Rational simplified() const;
    void simplify();

    Rational operator+(const Rational& rhs);
    Rational operator-(const Rational& rhs);
    Rational operator*(const Rational& rhs);
    Rational operator/(const Rational& rhs);
    bool operator==(const Rational& rhs);
    operator double() const;

private:
    int m_numerator;
    int m_denominator;
};

int GCF(int a, int b) // Euclid's GCF algorithm
{
    a = abs(a);
    b = abs(b);
    if (a < b) swap(a, b); // Ensure a >= b
    if (b == 0) return a;
    int r{1};
    while (r != 0) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

ostream& operator<<(ostream& os, const Rational& r)
{
    os << r.numerator() << "/" << r.denominator();
    return os;
}

Rational::Rational() : Rational(0, 1) {}

Rational::Rational(int numerator) : Rational(numerator, 1) {}

Rational::Rational(int numerator, int denominator):
    m_numerator(numerator),
    m_denominator(denominator)
{
    if (denominator == 0) throw Invalid();
}

int Rational::numerator() const { return m_numerator; }

int Rational::denominator() const { return m_denominator; }

Rational Rational::simplified() const
{
    int num{m_numerator}, denom{m_denominator};
    if (m_denominator < 0) {
        num *= -1;
        denom *= -1;
    }
    int gcf = GCF(m_numerator, m_denominator);
    num /= gcf;
    denom /= gcf;
    return Rational{num, denom};
}

void Rational::simplify()
{
    Rational s = simplified();
    m_numerator = s.numerator();
    m_denominator = s.denominator();
}

Rational Rational::operator+(const Rational &rhs)
{
    int num = m_numerator * rhs.m_denominator + m_denominator * rhs.m_numerator;
    int denom = m_denominator * rhs.m_denominator;
    return Rational{num, denom};
}

Rational Rational::operator-(const Rational &rhs)
{
    int num = m_numerator * rhs.m_denominator - m_denominator * rhs.m_numerator;
    int denom = m_denominator * rhs.m_denominator;
    return Rational{num, denom};
}

Rational Rational::operator*(const Rational &rhs)
{
    int num = m_numerator * rhs.m_numerator;
    int denom = m_denominator * rhs.m_denominator;
    return Rational{num, denom};
}

Rational Rational::operator/(const Rational &rhs)
{
    int num = m_numerator * rhs.m_denominator;
    int denom = m_denominator * rhs.m_numerator;
    return Rational{num, denom};
}

bool Rational::operator==(const Rational &rhs)
{
    Rational a = this->simplified();
    Rational b = rhs.simplified();
    return (a.m_numerator == b.m_numerator && a.m_denominator == b.m_denominator);
}

Rational::operator double() const { return static_cast<double>(m_numerator) / m_denominator; }



int main()
{
    return 0;
}
