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