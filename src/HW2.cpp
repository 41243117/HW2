#include <iostream>
#include <cmath>
using namespace std;

class Polynomial {
private:
    struct Term {
        float coef;
        int exp;
    };
    Term* termArray;
    int terms;
    int capacity;

    void ensureCapacity(int newCapacity) {
        if (newCapacity > capacity) {
            capacity = newCapacity;
            Term* newArray = new Term[capacity];
            for (int i = 0; i < terms; ++i) {
                newArray[i] = termArray[i];
            }
            delete[] termArray;
            termArray = newArray;
        }
    }

public:
    Polynomial(int cap = 10) : terms(0), capacity(cap) {
        termArray = new Term[capacity];
    }

    float Eval(float f) const {
        float result = 0;
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * pow(f, termArray[i].exp);
        }
        return result;
    }

    void AddTerm(float coef, int exp) {
        if (coef == 0) return;
        for (int i = 0; i < terms; ++i) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) {
                    for (int j = i; j < terms - 1; ++j) {
                        termArray[j] = termArray[j + 1];
                    }
                    terms--;
                }
                return;
            }
        }

        if (terms == capacity) {
            ensureCapacity(capacity * 2);
        }
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    void Print() const {
        for (int i = 0; i < terms; i++) {
            if (i > 0 && termArray[i].coef > 0) cout << " + ";
            if (termArray[i].coef < 0) cout << " - ";
            cout << abs(termArray[i].coef) << "x^" << termArray[i].exp;
        }
        cout << endl;
    }

    Polynomial Add(const Polynomial& other) const {
        Polynomial result;
        result.ensureCapacity(terms + other.terms);
        for (int i = 0; i < terms; ++i) {
            result.AddTerm(termArray[i].coef, termArray[i].exp);
        }
        for (int i = 0; i < other.terms; ++i) {
            result.AddTerm(other.termArray[i].coef, other.termArray[i].exp);
        }
        return result;
    }

    Polynomial Mult(const Polynomial& other) const {
        Polynomial result;
        result.ensureCapacity(terms * other.terms);
        for (int i = 0; i < terms; ++i) {
            for (int j = 0; j < other.terms; ++j) {
                result.AddTerm(termArray[i].coef * other.termArray[j].coef, termArray[i].exp + other.termArray[j].exp);
            }
        }
        return result;
    }

    friend istream& operator>>(istream& is, Polynomial& poly) {
        cout << "Enter number of terms: ";
        is >> poly.terms;

        if (poly.terms > poly.capacity) {
            poly.ensureCapacity(poly.terms);
        }

        for (int i = 0; i < poly.terms; i++) {
            cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
            is >> poly.termArray[i].coef >> poly.termArray[i].exp;
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (int i = 0; i < poly.terms; i++) {
            if (i > 0 && poly.termArray[i].coef > 0) {
                os << " + ";
            }
            os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        }
        return os;
    }

    ~Polynomial() {
        delete[] termArray;
    }
};

int main() {
    Polynomial p1, p2;

    p1.AddTerm(3, 2);
    p1.AddTerm(2, 1);
    p1.AddTerm(1, 0);

    p2.AddTerm(1, 2);
    p2.AddTerm(2, 1);
    p2.AddTerm(3, 0);

    cout << "First Polynomial: " << p1 << endl;
    cout << "Second Polynomial: " << p2 << endl;

    Polynomial sum = p1.Add(p2);
    cout << "Sum: " << sum << endl;

    Polynomial product = p1.Mult(p2);
    cout << "Product: " << product << endl;

    float x = 2.0; 
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}