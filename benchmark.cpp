#include<iostream>
#include<set>
#include "RB_TREE.h"
#include<chrono>
using namespace std;

int *ar, n, iters;
long long duration1 = 0, duration2 = 0;

bool verify(rb_tree& tree, set<int> &s)   {
    bool r = 1;
    int h = tree.check_height();
    if (h == -1)    {
        r = 0;
        cout << "\nHeight not maintained!";
    }
    if (!tree.check_red())  {
        r = 0;
        cout << "\nDouble red problem!";
    }
    int n = tree.size();
    tree.inorder(ar);
    int i = 0;
    for (auto e: s) {
        if (ar[i++] != e) {
            cout << "\nInorder traversal not correct!";
            r = 0;
            break;
        }
    }
    return r;
}

void randomize(int n)  {
    while (n > 0)  {
        // choose a random index
        int idx = rand() % n;
        // swap
        swap(ar[idx], ar[--n]);
    }
}

long long eval_set()   {
    auto start = chrono::high_resolution_clock::now();
    // evaluation of std::set
    set<int> s;
    for (int i=0; i<n; ++i)
        s.insert(ar[i]);
    // cout << "\nSet size after insertion is " << s.size();

    // remove half the elements
    for (int i=0; i<n; i+=2)
        s.erase(ar[i]);
    
    // cout << "\nSet size after deletion is " << s.size();
    // delete all elements
    s.clear();
    return chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
}

long long eval_tree()  {
    auto start = chrono::high_resolution_clock::now();
    // evaluation of rb_tree
    rb_tree tree;
    for (int i=0; i<n; ++i)
        tree.insert(ar[i]);
    // cout << "\nTree size after insertion is " << tree.size();

    // remove half the elements
    for (int i=0; i<n; i+=2)
        tree.erase(ar[i]);
    
    // cout << "\nTree size after deletion is " << tree.size();
    // delete all elements
    tree.clear();
    return chrono::duration_cast<chrono::microseconds>(chrono::high_resolution_clock::now() - start).count();
}

void timeit()   {
    long long d1, d2;

    for (int i=0; i<n; ++i)
        ar[i] = (rand() * rand()) % (int)(1e9+7);

    d1 = eval_set();
    d2 = eval_tree();
    duration1 += d1;
    duration2 += d2;
    // printf("\nTime taken for std::set operations: %llf seconds", d1 / 1e6);
    // printf("\nTime taken for rb_tree operations: %llf seconds\n\n", d2 / 1e6);
}

int main()  {
    n = 100000;
    ar = new int[n];
    iters = 20;
    // cout << "Enter the number of elements: ";
    // cin >> n;
    // cout << "Evaluate tree or set? (1 for set, 0 for tree): ";
    // cin >> eval_set;
    while (iters--) {
        timeit();
        cout << iters << ".";
    }
    printf("\nstd::set time: %llf seconds.", duration1 / 1e6);
    printf("\nrb_tree time: %llf seconds.", duration2 / 1e6);
    return 0;
}
