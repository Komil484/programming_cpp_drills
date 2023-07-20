#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;


class NamePair
{
public:
    NamePair();
    void addEntry(string name, int age);
    void readNames();
    void readAges();
    void sort();
    string getName(int i) const;
    int getAge(int i) const;
    int getSize() const;
private:
    vector<string> names;
    vector<double> ages;
};

NamePair::NamePair() {}

void NamePair::addEntry(string name, int age)
{
    names.push_back(name);
    ages.push_back(age);
}

void NamePair::readNames()
{
    string name;
    cout << "Input names (\".\" ends input)\n";
    getline(cin, name);
    while (name != ".") {
        names.push_back(name);
        getline(cin, name);
    }
}

void NamePair::readAges()
{
    int age;
    cout << "Input ages\n";
    for (int i = 0; i < names.size(); i++) {
        do {
            cout << names[i] << ": ";
            cin >> age;
        } while (cin.fail());
        try {
            ages.insert(ages.begin() + i, age);
        } catch (...) {
            cout << "An error has occured";
        }
    }
}

void NamePair::sort()
{
    for (int i = 0; i < getSize() - 1; i++) {
        for (int j = 0; j < getSize() - i - 1; j++) {
            if (names[j] > names[j + 1]) {
                swap(names[j], names[j + 1]);
                swap(ages[j], ages[j + 1]);
            }
        }
    }
}

string NamePair::getName(int i) const
{
    return names.at(i);
}

int NamePair::getAge(int i) const
{
    return ages.at(i);
}

int NamePair::getSize() const
{
    return names.size();
}

ostream& operator<<(ostream& os, const NamePair& namePairObj)
{
    for (int i = 0; i < namePairObj.getSize(); i++) {
        os << namePairObj.getName(i) << ": " << namePairObj.getAge(i) << '\n';
    }
    return os;
}

bool operator==(const NamePair& a, const NamePair& b)
{
    if (a.getSize() != b.getSize()) return false;
    for (int i = 0; i < a.getSize(); i++) {
        if (a.getName(i) != b.getName(i)) return false;
        if (a.getAge(i) != b.getAge(i)) return false;
    }
    return true;
}

bool operator!=(const NamePair& a, const NamePair& b)
{
    return !(a == b);
}

int main()
{
    NamePair namePairObj;
    namePairObj.readNames();
    namePairObj.readAges();
    namePairObj.sort();
    cout << "Names and ages:\n";
    cout << namePairObj << "\n";
    return 0;
}
