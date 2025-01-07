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
    Polynomial copy(const Polynomial& a) {
        head = new Node;
        head->coef = 0;
        head->exp = -1;
        head->link = head;

        Node* tail = head;
        Node* curr = a.head->link;
        while (curr != a.head) {
            Node* newNode = new Node;
            newNode->coef = curr->coef;
            newNode->exp = curr->exp;
            newNode->link = head;
            tail->link = newNode;
            tail = newNode;
            curr = curr->link;
        }
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

    double x;
    cout << "輸入 x 值: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Evaluate(x) << endl;
    cout << "p2(" << x << ") = " << p2.Evaluate(x) << endl;
    cout << "(p1 + p2)(" << x << ") = " << sum.Evaluate(x) << endl;
    cout << "(p1 - p2)(" << x << ") = " << diff.Evaluate(x) << endl;
    cout << "(p1 * p2)(" << x << ") = " << mule.Evaluate(x) << endl;
    return 0;
}
/*作業 3

[程式設計專案]
開發一個 C++ 類別 Polynomial，用來表示和操作具有整數係數的單變數多項式（使用具有頭節點的循環鏈結串列）。每個多項式的每一項將表示為一個節點。因此，這個系統中的節點將有以下三個資料成員：

coef : 係數
exp : 指數
link : 指向下一個節點的指標
每個多項式應表示為具有頭節點的循環鏈結串列。為了有效地刪除多項式，需要使用第 4.5 節中描述的可用空間表（available - space list）和相關函數。單變數多項式的外部（即用於輸入或輸出）表示將假設為由以下形式的整數序列組成：
n, c₁, e₁, c₂, e₂, c₃, e₃, ..., cₙ, eₙ，其中 eᵢ 代表指數，cᵢ 代表係數，n 表示多項式的項數。指數以遞減順序排列：
e₁ > e₂ > ... > eₙ。

撰寫並測試以下函數：

istream& operator>>(istream& is, Polynomial& x) :
    從輸入中讀取多項式並將其轉換為使用頭節點的循環鏈結串列表示。

    ostream& operator<<(ostream& os, Polynomial& x) :
    將多項式從鏈結串列表示轉換為其外部表示並輸出。

    Polynomial::Polynomial(const Polynomial& a) :
    [拷貝建構子]
    將多項式 a 初始化到多項式* this。

    const Polynomial& Polynomial::operator=(const Polynomial& a) const :
    [賦值運算子]
    將多項式 a 賦值給多項式* this。

    Polynomial::~Polynomial() :
    [解構子]
    將多項式* this 的所有節點返回到可用空間表。

    Polynomial operator+(const Polynomial& b) :
    [加法]
    創建並返回多項式* this + b。

    Polynomial operator-(const Polynomial & b) :
    [減法]
    創建並返回多項式 * this - b。

    Polynomial operator(const Polynomial & b) : *
    [乘法]
    創建並返回多項式 * this * b。

    Polynomial::Evaluate(float x) const :
    [多項式求值]
    計算並返回多項式 * this 在 x 處的值。*/




