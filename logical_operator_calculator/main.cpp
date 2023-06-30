#include <iostream>

using namespace std;

class Token{
public:
    char kind;
    int value = 0;
};

class Token_stream{
public:
    Token get();
    void putback(Token t);
private:
    bool full{false};
    Token buffer;
};

Token Token_stream::get()
{
    if (full) {
        full = false;
        return buffer;
    }
    char ch;
    cin >> ch;
    switch (ch) {
    case '!': case '~': case '|': case '^': case '&': case '(': case ')': case ';': case 'q':
        return Token{ch};
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        cin.putback(ch);
        int val;
        cin >> val;
        return Token{'0', val};
    default:
        throw runtime_error("Invalid input '" + string{ch} + "'");
    }
}

void Token_stream::putback(Token t)
{
    if (full) throw runtime_error("putback() into full buffer");
    full = true;
    buffer = t;
}

Token_stream ts;

int expression();

int primary()
{
    Token t = ts.get();
    switch (t.kind) {
    case '(': case ')': {
        int group = expression();
        t = ts.get();
        if (t.kind != ')') throw runtime_error("Expected ')'");
        return group;
;    }
    case '!': case '~':
        return ~primary();
    case '0':
        return t.value;
    default:
        cout << "T: (" << t.kind << ", " << t.value << ")";
        cout.flush();
        throw runtime_error("Primary required");
    }
}

int secondary()
{
    int left = primary();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '&':
            left = left & primary();
            break;
        default:
            ts.putback(t);
            return left;
        }
        t = ts.get();
    }
}

int tertiary()
{
    int left = secondary();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '^':
            left = left ^ secondary();
            break;
        default:
            ts.putback(t);
            return left;
        }
        t = ts.get();
    }
}

int expression()
{
    int left = tertiary();
    Token t = ts.get();
    while (true) {
        switch (t.kind) {
        case '|':
            left = left | tertiary();
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
    int result = 0;
    do {
        Token t = ts.get();
        switch (t.kind) {
        case 'q':
            return 0;
        case ';':
            cout << result << "\n";
            break;
        default:
            ts.putback(t);
            result = expression();
            break;
        }
    } while (cin);
    return 2;
}
