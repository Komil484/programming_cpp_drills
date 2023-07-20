#include <iostream>
#include <vector>
#include <regex>

using namespace std;

class Date {
public:
    enum class Month {
        Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    };

    class Invalid : runtime_error {
        friend class Date;
        Invalid() : runtime_error("Invalid Date") {}
        Invalid(string what_arg) : runtime_error(what_arg) {};
    };

    Date() : Date(1970, Month::Jan, 1) {};
    Date(int year, Month month, int day);

    int   year()  const { return m_year; }
    Month month() const { return m_month; }
    int   day()   const { return m_day; }

    void nextDay();

private:
    int m_year;
    Month m_month;
    int m_day;
};

bool isDate(int year, Date::Month month, int day);

bool isLeapYear(int year);

bool operator==(const Date& a, const Date& b);
bool operator!=(const Date& a, const Date& b);

ostream& operator<<(ostream& os, const Date& d);
istream& operator>>(istream& is, Date& dd);

Date::Date(int year, Month month, int day) :
    m_year(year), m_month(month), m_day(day)
{
    if (!isDate(year, month, day)) throw Invalid();
}

int daysInMonth(int y, Date::Month m);

void Date::nextDay()
{
    if (m_day == daysInMonth(m_year, m_month)) {
        m_day = 1;
        if (m_month == Month::Dec) {
            m_month = Month::Jan;
            m_year++;
        } else {
            m_month = Month(static_cast<int>(m_month) + 1);
        }
    } else {
        m_day++;
    }
}

int daysInMonth(int year, Date::Month month)
{
    static const vector<int> table_daysInMonth {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (month == Date::Month::Feb && isLeapYear(year)) return 29;
    else return table_daysInMonth[static_cast<int>(month) - 1];
}

bool isDate(int year, Date::Month  month, int day)
{
    if (year <= 0) return false;
    if (day <= 0) return false;
    if (day > daysInMonth(year, month)) return false;
    return true;
}

bool isLeapYear(int y)
{
    if (y % 400 == 0) return true;
    if (y % 100 == 0) return false;
    if (y % 4 == 0) return true;
    return false;
}

bool operator==(const Date& a, const Date& b)
{
    return a.year()==b.year()
           && a.month()==b.month()
           && a.day()==b.day();
}

bool operator!=(const Date& a, const Date& b)
{
    return !(a==b);
}

ostream& operator<<(ostream& os, const Date& d)
{
    return os << '(' << d.year()
              << ',' << static_cast<int>(d.month())
              << ',' << d.day()
              << ')';
}

class Book {
public:
    enum class Genre {
        Fiction, Nonfiction, Periodical, Biography, Children
    };

    Book(const string &ISBN, const string &title, const string &author, const string &copyrightDate, Genre genre, bool isCheckedOut);


    string ISBN() const;
    string title() const;
    string author() const;
    string copyrightDate() const;
    Genre genre() const;
    bool isCheckedOut() const;
    void setIsCheckedOut(bool newIsCheckedOut);

private:
    string m_ISBN;
    string m_title;
    string m_author;
    string m_copyrightDate;
    Genre m_genre;
    bool m_isCheckedOut;
};

Book::Book(const string& ISBN, const string& title, const string& author, const string& copyrightDate, Book::Genre genre, bool isCheckedOut) :

    m_ISBN(ISBN),
    m_title(title),
    m_author(author),
    m_copyrightDate(copyrightDate),
    m_genre(genre),
    m_isCheckedOut(isCheckedOut)
{
    static const regex re{R"(([1-9]\d*)-([1-9]\d*)-([1-9]\d*)-([0-9A-Za-z]))"};

    if (!regex_match(ISBN, re)) throw invalid_argument{"Invalid ISBN: " + ISBN};
}

string Book::ISBN() const
{
    return m_ISBN;
}

string Book::title() const
{
    return m_title;
}

string Book::author() const
{
    return m_author;
}

string Book::copyrightDate() const
{
    return m_copyrightDate;
}

Book::Genre Book::genre() const
{
    return m_genre;
}

bool Book::isCheckedOut() const
{
    return m_isCheckedOut;
}

void Book::setIsCheckedOut(bool newIsCheckedOut)
{
    m_isCheckedOut = newIsCheckedOut;
}

bool operator==(const Book& a, const Book& b)
{
    return (a.ISBN() == b.ISBN());
}

bool operator!=(const Book& a, const Book& b)
{
    return !(a == b);
}

ostream& operator<<(ostream& os, const Book& book)
{
    os << "Title: " << book.title() << "\n";
    os << "Author: " << book.author() << "\n";
    os << "ISBN: " << book.ISBN() << "\n";
    return os;
}


class Patron {
public:
    Patron(const string& userName, const string& cardNumber);

    string userName() const;
    string cardNumber() const;
    double fees() const;
    void setFees(double newFees);

private:
    string m_userName;
    string m_cardNumber;
    double m_fees{0};
};

Patron::Patron(const string& userName, const string& cardNumber) :
    m_userName(userName),
    m_cardNumber(cardNumber) {}

string Patron::userName() const
{
    return m_userName;
}

string Patron::cardNumber() const
{
    return m_cardNumber;
}

double Patron::fees() const
{
    return m_fees;
}

void Patron::setFees(double newFees)
{
    m_fees = newFees;
}

bool owesFee(const Patron& patron)
{
    return (patron.fees() > 0);
}


int main()
{

    Book my_book{"123-2-3-4", "My Book", "I", "2022", Book::Genre::Fiction, false};
    Book his_book{"223-5-5-1", "His Book", "He", "2023", Book::Genre::Biography, true};
    Book our_book{"321-6-7-2", "Our Book", "We", "2021", Book::Genre::Children, false};

    cout << my_book;
    cout << his_book;
    cout << our_book;

    return 0;
}
