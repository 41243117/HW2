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

        // 計算多項式的值
        for (int i = 0; i < terms; i++) {
            result += termArray[i].coef * pow(f, termArray[i].exp);
        }

        return result;
    }

    // 添加一個新項到多項式
    void AddTerm(float coef, int exp) {
        if (coef == 0) return; // 如果係數為 0 則忽略

        // 檢查是否已經存在相同指數的項
        for (int i = 0; i < terms; ++i) {
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (termArray[i].coef == 0) {
                    // 如果係數變為 0，則移除該項
                    for (int j = i; j < terms - 1; ++j) {
                        termArray[j] = termArray[j + 1];
                    }
                    terms--;
                }
                return;
            }
        }

        // 如果數組已滿，擴展數組
        if (terms == capacity) {
            ensureCapacity(capacity * 2);
        }

        // 添加新項
        termArray[terms].coef = coef;
        termArray[terms].exp = exp;
        terms++;
    }

    // 輸出多項式
    void Print() const {
        for (int i = 0; i < terms; i++) {
            if (i > 0 && termArray[i].coef > 0) cout << " + ";
            if (termArray[i].coef < 0) cout << " - ";
            cout << abs(termArray[i].coef) << "x^" << termArray[i].exp;
        }
        cout << endl;
    }

    // 多項式加法
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

    // 多項式乘法
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

    // 輸入運算符重載
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

    // 輸出運算符重載
    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (int i = 0; i < poly.terms; i++) {
            if (i > 0 && poly.termArray[i].coef > 0) {
                os << " + ";
            }
            os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
        }
        return os;
    }

    // 析構函數：釋放動態數組
    ~Polynomial() {
        delete[] termArray;
    }
};

// 主函數測試
int main() {
    Polynomial p1, p2;

    // 固定輸入第一個多項式 p1(x) = 3x^2 + 2x + 1
    p1.AddTerm(3, 2);
    p1.AddTerm(2, 1);
    p1.AddTerm(1, 0);

    // 固定輸入第二個多項式 p2(x) = x^2 + 2x + 3
    p2.AddTerm(1, 2);
    p2.AddTerm(2, 1);
    p2.AddTerm(3, 0);

    cout << "First Polynomial: " << p1 << endl;
    cout << "Second Polynomial: " << p2 << endl;

    // 測試多項式加法
    Polynomial sum = p1.Add(p2);
    cout << "Sum: " << sum << endl;

    // 測試多項式乘法
    Polynomial product = p1.Mult(p2);
    cout << "Product: " << product << endl;

    float x = 2.0; // 固定輸入 x 的值
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}
