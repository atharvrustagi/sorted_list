#include<iostream>
#include<set>
#include<queue>
#include<vector>
#include "RB_TREE.H"
using namespace std;

void randomize(vector<int> &ar)  {
    int n = ar.size();
    while (n > 0)  {
        // choose a random index
        int idx = rand() % n;
        // swap
        swap(ar[idx], ar[--n]);
    }
}

int main()  {
    rb_tree t;
    vector<int> ar;
    int n;
    cin >> n;
    for (int i=1; i<=n; ++i)
        ar.push_back(i);
    randomize(ar);
    for (int i=0; i<n; ++i)
        t.insert(ar[i]);
    t._print();
    for (int i=0; i<n; ++i) {
        cout << t.at(i) << " ";
    }    
    return 0;
}
