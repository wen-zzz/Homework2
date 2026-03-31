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