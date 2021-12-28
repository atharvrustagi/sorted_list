#include<iostream>
#include<set>
#include<vector>
#include "sorted_list.h"
using namespace std;

bool verify(sorted_list& tree, set<int> &s)   {
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
    if (!tree.check_counts())   {
        r = 0;
        cout << "\nCounts not valid!";
    }
    int n = tree.size();
    int ar[n];
    tree.inorder(ar);
    int i = 0;
    // cout << endl;
    for (int e: s) {
        // cout << ar[i] << " ";
        if (ar[i++] != e) {
            cout << "\nInorder traversal not correct!";
            r = 0;
            break;
        }
    }
    return r;
}

bool test1(int t_no)    {
    // constructor and destructor test 1
    printf("Running test %d: ", t_no);
    sorted_list t;
    return 1;
}

bool test2(int t_no)    {
    // constructor and destructor test 2
    printf("Running test %d: ", t_no);
    sorted_list t;
    t.clear();
    return 1;
}

bool test3(int t_no)    {
    // insertion test 1
    printf("Running test %d: ", t_no);
    sorted_list t;
    t.clear();
    t.insert(5);
    return 1;
}

bool test4(int t_no)    {
    // insertion and deletion test
    printf("Running test %d: ", t_no);
    sorted_list t;
    t.insert(5);
    t.erase(5);
    return 1;
}

bool test5(int t_no)    {
    // insertion and deletion test
    printf("Running test %d: ", t_no);
    sorted_list t;
    t.insert(5);
    t.erase(5);
    t.clear();
    return 1;
}

bool test6(int t_no)    {
    // deletion test
    printf("Running test %d: ", t_no);
    sorted_list t;
    t.erase(5);
    return 1;
}

bool test7(int t_no)    {
    // heavy insertion and deletion test
    printf("Running test %d: ", t_no);
    int n = 5000;
    vector<int> test;
    for (int i=0; i<n; ++i)
        test.push_back(rand() % rand());
    
    // new set
    set<int> s;
    // new sorted_list
    sorted_list t;

    // insert all elements and check the traversal for each iteration
    for (int v: test) {
        t.insert(v);
        s.insert(v);
        // check traversal
        if (!verify(t, s))
            return 0;
    }

    // delete half the elements and check the traversal for each iteration
    for (int j=0; j<n; j+=2) {
        t.erase(test[j]);
        s.erase(test[j]);
        // check traversal
        if (!verify(t, s))
            return 0;
    }

    // insert deleted elements again and check the traversal for each iteration
    for (int i=0; i<n; i+=2) {
        t.insert(test[i]);
        s.insert(test[i]);
        // check traversal
        if (!verify(t, s))
            return 0;
    }

    return 1;
}

bool test8(int t_no)    {
    // retrieving at index test
    printf("Running test %d: ", t_no);
    int n = 5000;
    sorted_list t;
    set<int> s;
    for (int i=0; i<n; ++i) {
        int r = rand() % rand();
        s.insert(r);
        t.insert(r);
        // check traversal through retrievals
        int j = t.size()-1;
        for (auto it = s.rbegin(); it != s.rend(); ++it)
            if (*it != t[j--])
                return 0;
    }
    return 1;
}

bool test9(int t_no)    {
    // index_of test
    printf("Running test %d: ", t_no);
    int n = 5000;
    sorted_list t;
    set<int> s;
    for (int i=0; i<n; ++i) {
        int r = rand() % (rand() + 1);
        s.insert(r);
        t.insert(r);
        int idx = 0;
        for (int v: s)
            if (t.index_of(v) != idx++)
                return 0;
    }
    return 1;
}

void run_tests()    {
    typedef bool (*BoolTests) (int t);
    BoolTests tests[] = {test1, test2, test3, test4, test5, test6, test7, test8, test9};
    int n = sizeof(tests) / sizeof(tests[0]);
    bool t = 1;
    for (int i=0; i<n; ++i) {
        bool p = tests[i](i+1);
        cout << (p ? "Passed\n" : "Failed\n");
        t &= p;
    }
    if (t)  cout << "All test cases passed\n";
}

int main()  {
    run_tests();
    return 0;
}
