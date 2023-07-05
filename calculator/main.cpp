/*
--Grammar--
Calculation:
    Statement
    Print
    Quit
    Calculation Statement
Statement:
    Definition
    Expression
Definition:
    Name = Expression
Expression:
    Term
    Expression + Term
    Expression - Term
Term:
    Primary
    Term * Primary
    Term / Primary
    Term % Primary
Primary:
    Number
    ( Expression )
    - Primary
    + Primary
Number:
    floating-point-literal
*/


#include <iostream>
#include <map>
#include <cmath>
#include <cctype>

using namespace std;

const char number = '0';
const char quit = 'q';
const char print = ';';
const char name = 'a';
const char set = 's';
const string declaration_key = "set";
const string prompt = "> ";
const string result = "= ";


void error(string error_message)
{
    throw runtime_error{error_message};
}

class Token {
public:
    char kind;
    double value;
    string name;
    Token() : kind{0} {};
    Token(char ch) : kind{ch} {}
    Token(char ch, double d) : kind{ch}, value{d} {}
    Token(char ch, string s) : kind{ch}, name{s} {}
};

class Token_stream {
public:
    Token get();
    void putback(Token t);
    void ignore_until(char ignore_ch);
private:
    Token buffer;
    bool full{false};
};

void Token_stream::putback(Token t)
{
    if (full)
        error("Token buffer full");
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }
    char ch;
    cin >> ch;

    switch (ch) {
    case print: case quit:
    case '(': case ')': case '=':
    case '+': case '-': case '*': case '/': case '%':
        return Token{ch};
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token{number, val};
    }
    default:
        if (isalpha(ch)) {
            string s;

            s += ch;
            while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
                s += ch;
            cin.putback(ch);

            if (s == declaration_key)
                return Token{set};

            return Token{name, s};
        } else {
            error("Invalid token");
        }
    }
    return Token{};
}

void Token_stream::ignore_until(char ignore_ch)
{
    if (full && ignore_ch == buffer.kind) {
        full = false;
        return;
    }
    full = false;
    char in_ch;
    while (cin >> in_ch) {
        if (in_ch == ignore_ch)
            return;
    }
}

Token_stream ts;

map<string, int> var_map;

double expression();

bool is_var_declared(string var_name)
{
    return var_map.find(var_name) != var_map.end();
}

double get_var_value(string var_name)
{
    if (is_var_declared(var_name))
        return var_map[var_name];

    error(var_name + " is not defined");
    return 0.0;
}

void set_var_value(string var_name, double value)
{
    var_map[var_name] = value;
}

double var_definition()
{
    Token t = ts.get();
    if (t.kind != name)
        error ("name expected in variable definition");

    string var_name = t.name;

    t = ts.get();
    if (t.kind != '=')
        error("'=' missing in definition of " + var_name);

    int value = expression();

    set_var_value(var_name, value);
    return value;
}

double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(': {
        double d = expression();
        t = ts.get();
        if
            (t.kind != ')') error("')' expected");
        return d;
    }
    case number:
        return t.value;
    case '-':
        return -primary();
    case '+':
        return primary();
    case name:
        return get_var_value(t.name);
    default:
        error("Primary expected");
    }
    return 0.0;
}

double term()
{
    double left = primary();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '*':
            left *= primary();
            break;
        case '/': {
            double right = primary();
            if (right == 0)
                error("Division by 0");
            left /= right;
            break;
        }
        case '%': {
            double right = primary();
            if (right == 0)
                error("Division by 0");
            left = left - right*floor(left/right); // right % left
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
        t = ts.get();
    }
}

double expression()
{
    double left = term();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '+':
            left += term();
            break;
        case '-':
            left -= term();
            break;
        default:
            ts.putback(t);
            return left;
        }
        t = ts.get();
    }
}

double statement()
{
    Token t = ts.get();
    switch (t.kind) {
    case set:
        return var_definition();
    default:
        ts.putback(t);
        return expression();
    }
    return 0.0;
}

int main()
{
    double val = 0;
    Token t;

    set_var_value("pi", 3.1415926535);
    set_var_value("e", 2.7182818284);

    while (cin) {
        try {
            cout << prompt;
            t = ts.get();
            while (t.kind == print)
                t = ts.get();
            if (t.kind == quit)
                break;
            ts.putback(t);
            cout << result << statement() << "\n";
        } catch (exception &e) {
            cerr << e.what() << "\n";
            ts.ignore_until(print);
        } catch (...) {
            cerr << "Unknown exception\n";
            return 2;
        }
    }
}
