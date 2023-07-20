#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>

using namespace std;

class Money {
public:
    enum class Currency {
        USD, EUR, GBP, RUB
    };

    void convertTo(Currency newCurrency);
    Money convertedTo(Currency newCurrency) const;

    explicit Money(Currency currency = Currency::USD) : Money(0) {}
    explicit Money(long double value, Currency currency = Currency::USD):
        m_value{value},
        m_currency{currency} {}

    long double value() const;
    Currency currency() const;

    Money operator-() const;
    Money& operator+=(const Money& rhs);
    Money& operator-=(const Money& rhs);


private:
    long double m_value;
    Currency m_currency;
    static const map<Currency, long double> moneyConversionTable;
};

const map<Money::Currency, long double> Money::moneyConversionTable {
    {Money::Currency::USD, 1.0L},
    {Money::Currency::EUR, 0.8986L},
    {Money::Currency::GBP, 0.77749L},
    {Money::Currency::RUB, 90.4L}
};

Money Money::operator-() const
{
    return Money{-value(), currency()};
}

Money& Money::operator+=(const Money& rhs)
{
    auto newValue = rhs.convertedTo(currency()).value() + value();
    *this = Money{newValue, currency()};
    return *this;
}

Money& Money::operator-=(const Money& rhs)
{
    auto newValue = rhs.convertedTo(currency()).value() - value();
    *this = Money{newValue, currency()};
    return *this;
}

long double Money::value() const
{
    return m_value;
}

Money::Currency Money::currency() const
{
    return m_currency;

}

void Money::convertTo(Currency newCurrency)
{
    *this = convertedTo(newCurrency);
}

Money Money::convertedTo(Currency newCurrency) const
{
    auto temp_value = value();

    temp_value /= moneyConversionTable.at(currency());
    temp_value *= moneyConversionTable.at(newCurrency);

    return Money{temp_value, newCurrency};
}

string currencyToString(Money::Currency currency)
{
    static const map<Money::Currency, string> currencyToStringTable {
        {Money::Currency::USD, "USD"},
        {Money::Currency::EUR, "EUR"},
        {Money::Currency::GBP, "GBP"},
        {Money::Currency::RUB, "RUB"}
    };
    return currencyToStringTable.at(currency);
}

ostream& operator<<(ostream& os, const Money& money)
{
    stringstream ss;
    ss << fixed << setprecision(2) << money.value();
    os << ss.str() << " " << currencyToString(money.currency());
    return os;
}

int main()
{
    Money m{5, Money::Currency::USD};
    cout << m << " + ";
    Money m2{5, Money::Currency::EUR};
    cout << m2 << " = ";
    m += m2;
    cout << m << endl;

    return 0;
}
