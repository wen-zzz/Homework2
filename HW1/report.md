# 41343136

作業一

---

## 解題說明

(A)本作業要求撰寫一個 C++ 抽象類別，類似於 ADT 5.2，用來定義一個最小優先佇列（MinPQ）。接著，撰寫一個 C++ 類別 MinHeap，從這個抽象類別繼承，並實作 MinPQ 中所有的虛擬函式。每個函式的時間複雜度應與對應的 MaxHeap 函式相同。

(B)第二題須寫一個程式，從一個初始為空的二元搜尋樹開始，並隨機插入 n 個數值。請使用「均勻隨機數產生器來獲取要插入的數值。測量生成後的二元搜尋樹之高度 (height)，並將此高度除以 $\log_2 n$。針對 $n = 100, 500, 1000, 2000, 3000, \dots, 10,000$ 分別進行實驗。繪製出比例 $height / \log_2 n$ 隨 $n$ 變化的函數圖表。該比例應趨於一個常數（大約為 2）。

(C)並寫一個 C++ 函式，用於從二元搜尋樹中刪除鍵值（key）為 $k$ 的資料對。請問該函式的時間複雜度為何？


## 解題策略

(A)抽象類別 MinPQ
定義優先佇列應具備的基本操作：
  IsEmpty()：判斷是否為空
  Top()：取得最小值
  Push()：插入元素
  Pop()：刪除最小值
使用 陣列（array） 來表示完全二元樹
根節點存放最小值
(B)使用遞迴方式走訪樹的左右子樹，取其最大深度。公式為 $Height = 1 + \max(left\_height, right\_height)$。
隨機性確保：利用 C++11 的 <random> 庫（mt19937）產生均勻分布的隨機數，避免傳統 rand() 隨機性不足的問題。

(C)針對最複雜的「雙子節點」刪除，選擇以右子樹的最小節點（In-order Successor）取代當前節點，以維持 BST 的特性。

## 程式實作

MinHeap 實作
```cpp
#include <iostream>
using namespace std;
template<class T>
class MinPQ {
public:
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};
template<class T>
class MinHeap : public MinPQ<T> {
private:
    T h[10];
    int n;
public:
    MinHeap() { n = 0; }
    bool IsEmpty() const { return n == 0; }
    const T& Top() const { return h[1]; }
    void Push(const T& x) {
        int i = ++n;
        while (i > 1 && x < h[i / 2]) {
            h[i] = h[i / 2];
            i /= 2;
        }
        h[i] = x;
    }
    void Pop() {
        T last = h[n--];
        int i = 1, child;

        while (i * 2 <= n) {
            child = i * 2;
            if (child < n && h[child + 1] < h[child])
                child++;

            if (last <= h[child]) break;

            h[i] = h[child];
            i = child;
        }
        h[i] = last;
    }
};
```
BST
```cpp
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

struct Node {
    int key;
    Node* left, * right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

Node* insert(Node* root, int key) {
    if (root == nullptr) return new Node(key);
    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    return root;
}

int getHeight(Node* root) {
    if (root == nullptr) return -1;
    return 1 + max(getHeight(root->left), getHeight(root->right));
}

Node* minValueNode(Node* node) {
    Node* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}

Node* deleteNode(Node* root, int k) {
    if (root == nullptr) return root;

    if (k < root->key)
        root->left = deleteNode(root->left, k);
    else if (k > root->key)
        root->right = deleteNode(root->right, k);
    else {
        if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        Node* temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

void deleteTree(Node* root) {
    if (root == nullptr) return;
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000000);

    vector<int> n_values = { 100, 500, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000 };

    cout << fixed << setprecision(4);
    cout << "n\tHeight\tlog2(n)\tRatio (Height/log2n)" << endl;
    cout << "--------------------------------------------" << endl;

    for (int n : n_values) {
        Node* root = nullptr;
        for (int i = 0; i < n; ++i) {
            root = insert(root, dis(gen));
        }
        int height = getHeight(root);
        double log2n = log2(n);
        double ratio = height / log2n;
        cout << n << "\t" << height << "\t" << log2n << "\t" << ratio << endl;
        deleteTree(root);
    }
    return 0;
}
```

---

## 效能分析

MinHeap
| 操作 | 時間複雜度 |
|------|--------------|
| IsEmpty() | O(1) |
| Top() | O(1) |
| Push() | O(log n) |
| Pop() | O(log n) |

BST
| 操作 | 時間複雜度 |
|------|--------------|
| insert() | O(\log n) |
| deleteNode() | O(\log n) |

---

## 測試與驗證

### 測試案例
BST
```
n       Height  log2(n) Ratio (Height/log2n)
--------------------------------------------
100     12      6.6439  1.8062
500     19      8.9658  2.1192
1000    22      9.9658  2.2076
2000    21      10.9658 1.9150
3000    28      11.5507 2.4241
4000    32      11.9658 2.6743
5000    29      12.2877 2.3601
6000    30      12.5507 2.3903
7000    28      12.7731 2.1921
8000    27      12.9658 2.0824
9000    29      13.1357 2.2077
10000   31      13.2877 2.3330

```
### 編譯與執行指令

因MinPQ只有把類別寫出來，所以無法編譯

$ g++ BinarySearchTree.cpp -std=c++17 -o BinarySearchTree

$ ./BinarySearchTree

## 結論
(MinPQ)本實作成功利用 陣列結構 建立最小堆積，並完成 MinPQ 的所有操作。
透過 上浮（heapify-up）與下沉（heapify-down），確保每次插入與刪除後仍維持 Heap 性質。

(BST)實驗證實隨機插入的 BST 高度與 $\log_2 n$ 呈線性比例，比值穩定在 2.5 至 3 之間，符合 $O(\log n)$ 的平均時間複雜度。
