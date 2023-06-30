#include <iostream>
#include <vector>

using namespace std;

void error(string error_message)
{
    throw runtime_error{error_message};
}

class Token {
public:
    char kind;
    double value;
};

class Token_stream {
public:
    Token get();
    void putback(Token t);
private:
    Token buffer;
    bool full{false};
};

void Token_stream::putback(Token t)
{
    if (full) error("Token buffer full");
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
    case ';': case 'q':
    case '(': case ')': case '+': case '-': case '*': case '/':
        return Token{ch};
    case '.':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9': {
        cin.putback(ch);
        double val;
        cin >> val;
        return Token{'0', val};
    }
    default:
        error("Invalid token");
    }
    return Token{'_'};
}

Token_stream ts;

double expression();

double primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(': {
        double d = expression();
        t = ts.get();
        if (t.kind != ')') error("')' expected");
        return d;
    }
    case '0':
        return t.value;
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
            if (right == 0) error("Division by 0");
            left /= right;
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

int main()
{
    double result = 0;
    Token t;

    do {
        t = ts.get();
        switch (t.kind) {
        case 'q':
            return 0;
        case ';':
            cout << result << '\n';
            break;
        default:
            ts.putback(t);
            result = expression();
        }
    } while (cin);

    error("Invalid expression");
}
