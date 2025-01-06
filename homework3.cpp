#include <iostream>
#include <cmath>
using namespace std;

// 成員結構
struct Node {
    int coef;    // 係數
    int exp;     // 指數
    Node* link;  // 指向下一個節點的指標
};

class Polynomial {
private:
    Node* head;

    // 清除所有節點
    void clear() {
        Node* curr = head->link;
        while (curr != head) {
            Node* temp = curr;
            curr = curr->link;
            delete temp;
        }
        head->link = head;
    }

public:
    // 預設建構子
    Polynomial() {
        head = new Node;
        head->coef = 0;
        head->exp = -1;
        head->link = head;
    }

    // 解構子
    ~Polynomial() {
        clear();
        delete head;
        head = nullptr;
    }

    // 拷貝建構子
    Polynomial(const Polynomial& other) {
        head = new Node;
        head->coef = 0;
        head->exp = -1;
        head->link = head;
        *this = other;
    }

    // 賦值運算子
    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        clear();
        Node* tail = head;
        Node* oCurr = other.head->link;
        while (oCurr != other.head) {
            Node* newNode = new Node;
            newNode->coef = oCurr->coef;
            newNode->exp = oCurr->exp;
            newNode->link = head;
            tail->link = newNode;
            tail = newNode;
            oCurr = oCurr->link;
        }
        return *this;
    }

    // 讀取多項式
    istream& readPolynomial(istream& is) {
        int n;
        is >> n;
        Node* tail = head;
        for (int i = 0; i < n; i++) {
            int c, e;
            is >> c >> e;
            Node* newNode = new Node;
            newNode->coef = c;
            newNode->exp = e;
            newNode->link = head;
            tail->link = newNode;
            tail = newNode;
        }
        return is;
    }

    // 輸出多項式
    ostream& printPolynomial(ostream& os) const {
        Node* curr = head->link;
        bool isFirst = true;
        while (curr != head) {
            if (!isFirst) {
                if (curr->coef > 0) os << " + ";
                else os << " - ";
            }
            else {
                if (curr->coef < 0) os << "-";
            }

            int absC = abs(curr->coef);
            if (absC != 1 || curr->exp == 0) os << absC;

            if (curr->exp > 0) {
                os << "x";
                if (curr->exp > 1) os << "^" << curr->exp;
            }
            isFirst = false;
            curr = curr->link;
        }
        return os;
    }

    // 加法運算
    Polynomial operator+(const Polynomial& b) const {
        Polynomial result;
        Node* tail = result.head;
        Node* p1 = this->head->link;
        Node* p2 = b.head->link;

        while (p1 != this->head || p2 != b.head) {
            int c = 0, e = 0;
            if (p1 == this->head) {
                c = p2->coef;
                e = p2->exp;
                p2 = p2->link;
            }
            else if (p2 == b.head) {
                c = p1->coef;
                e = p1->exp;
                p1 = p1->link;
            }
            else {
                if (p1->exp > p2->exp) {
                    c = p1->coef;
                    e = p1->exp;
                    p1 = p1->link;
                }
                else if (p1->exp < p2->exp) {
                    c = p2->coef;
                    e = p2->exp;
                    p2 = p2->link;
                }
                else {
                    c = p1->coef + p2->coef;
                    e = p1->exp;
                    p1 = p1->link;
                    p2 = p2->link;
                }
            }
            if (c != 0) {
                Node* newNode = new Node;
                newNode->coef = c;
                newNode->exp = e;
                newNode->link = result.head;
                tail->link = newNode;
                tail = newNode;
            }
        }
        return result;
    }

    // 減法運算
    Polynomial operator-(const Polynomial& b) const {
        Polynomial result;
        Node* tail = result.head;
        Node* p1 = this->head->link;
        Node* p2 = b.head->link;

        while (p1 != this->head || p2 != b.head) {
            int c = 0, e = 0;
            if (p1 == this->head) {
                c = -p2->coef;
                e = p2->exp;
                p2 = p2->link;
            }
            else if (p2 == b.head) {
                c = p1->coef;
                e = p1->exp;
                p1 = p1->link;
            }
            else {
                if (p1->exp > p2->exp) {
                    c = p1->coef;
                    e = p1->exp;
                    p1 = p1->link;
                }
                else if (p1->exp < p2->exp) {
                    c = -p2->coef;
                    e = p2->exp;
                    p2 = p2->link;
                }
                else {
                    c = p1->coef - p2->coef;
                    e = p1->exp;
                    p1 = p1->link;
                    p2 = p2->link;
                }
            }
            if (c != 0) {
                Node* newNode = new Node;
                newNode->coef = c;
                newNode->exp = e;
                newNode->link = result.head;
                tail->link = newNode;
                tail = newNode;
            }
        }
        return result;
    }

    // 乘法運算
    Polynomial operator*(const Polynomial& b) const {
        Polynomial result;
        Node* p1 = this->head->link;

        while (p1 != this->head) {
            Node* p2 = b.head->link;
            while (p2 != b.head) {
                int c = p1->coef * p2->coef;
                int e = p1->exp + p2->exp;

                // 合併同指數的項
                Node* curr = result.head->link;
                Node* prev = result.head;
                bool found = false;
                while (curr != result.head) {
                    if (curr->exp == e) {
                        curr->coef += c;
                        found = true;
                        break;
                    }
                    prev = curr;
                    curr = curr->link;
                }

                if (!found) {
                    Node* newNode = new Node;
                    newNode->coef = c;
                    newNode->exp = e;
                    newNode->link = result.head;
                    prev->link = newNode;
                }

                p2 = p2->link;
            }
            p1 = p1->link;
        }

        return result;
    }

    // 多項式求值
    float Evaluate(float x) const {
        float sum = 0.0f;
        Node* curr = head->link;
        while (curr != head) {
            sum += curr->coef * pow(x, curr->exp);
            curr = curr->link;
        }
        return sum;
    }
};

// 多項式輸入運算子
istream& operator>>(istream& is, Polynomial& p) {
    return p.readPolynomial(is);
}

// 多項式輸出運算子
ostream& operator<<(ostream& os, const Polynomial& p) {
    return p.printPolynomial(os);
}

int main() {
    Polynomial p1, p2;
    cout << "輸入第一個多項式:(第一個數輸入項數 後面就輸入係數和指數) ";
    cin >> p1;
    cout << "輸入第二個多項式:(第一個數輸入項數 後面就輸入係數和指數) ";
    cin >> p2;

    cout << "p1 = " << p1 << endl;
    cout << "p2 = " << p2 << endl;

    Polynomial sum = p1 + p2;
    Polynomial diff = p1 - p2;
    Polynomial mule = p1 * p2;

    cout << "p1 + p2 = " << sum << endl;
    cout << "p1 - p2 = " << diff << endl;
    cout << "p1 * p2 = " << mule << endl;

    float x;
    cout << "輸入 x 值: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "p2(" << x << ") = " << p2.Evaluate(x) << endl;
    cout << "(p1 + p2)(" << x << ") = " << sum.Evaluate(x) << endl;
    cout << "(p1 - p2)(" << x << ") = " << diff.Evaluate(x) << endl;
    cout << "(p1 * p2)(" << x << ") = " << mule.Evaluate(x) << endl;
    return 0;
}
