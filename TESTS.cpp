#include<iostream>
#include<set>
#include<queue>
#include<vector>
// #include "RB_TREE.H"
using namespace std;


#define ll long long

// Node of a Red-Black Tree
struct Node {
    bool red;       // color of node (1 if red, 0 if black)
    int val;        // key value
    int count;      // number of keys in this subtree
    Node *left, *right;
    Node(int v, int r): val(v), red(r), count(1), left(nullptr),  right(nullptr) {}
};

// Red-Black tree implementation
class rb_tree   {
    Node *root;
    int tree_size;

    ll abs(ll res)  {
        return res>>63 ? -res : res;
    }

    void swap(bool &r1, bool &r2)   {
        if (r1 ^ r2)    {
            if (r1) {
                r1 = 0;
                r2 = 1;
            }
            else    {
                r1 = 1;
                r2 = 0;
            }
        }
    }

    ll _erase(int &key, Node* root, bool skip = 0, bool pass = 0) {
        ll res;
        bool from_left = 0;
        if (skip)   {
            from_left = pass;
            res = -1;
            goto skipped;
        }
        --root->count;
        // locate the key
        if (key < root->val)    {
            res = _erase(key, root->left);
            root->left = (res == -1) ? nullptr : (Node*)abs(res);
            from_left = 1;
        }
        else if (key > root->val)   {
            res = _erase(key, root->right);
            root->right = (res == -1) ? nullptr : (Node*)abs(res);
        }
        // this is the key
        else    {
            // if this is a leaf node
            if (!root->left && !root->right)    {
                bool r = root->red;
                // delete this node
                delete root;
                // best case, red node with no children
                if (r)
                    return 0;

                // black node
                // indicate to parent that there is double black problem
                return -1;
            }

            // root has only 1 child, so it cannot be black (think about it)
            // child is red if it exists, replace the node with child
            if (root->left)  {
                root->val = root->left->val;
                delete root->left;
                root->left = nullptr;
                return (ll)root;
            }
            if (root->right) {
                root->val = root->right->val;
                delete root->right;
                root->right = nullptr;
                return (ll)root;
            }
        }

        if (res >= 0)
            return (ll)root;

        skipped:
        // handle double black problem
        if (from_left)  {   // check right subtree
            Node* rchild = root->right;
            // case - 1, sibling is black
            if (!rchild->red)   {
                // case - 1.1, sibling is black with black children
                if ((!rchild->left || !rchild->left->red) && (!rchild->right || !rchild->right->red))   {
                    // make sibling red
                    rchild->red = 1;
                    if (root->red)  {
                        // make root black
                        root->red = 0;
                        return (ll)root;    // problem solved
                    }
                    // root is black, double black problem still exists
                    return -(ll)root;
                }

                // case - 1.2, near child of sibling is red
                if (rchild->left && rchild->left->red) {
                    // perform right rotation, and exchange colors
                    rchild = rotate_right(rchild);
                    root->right = rchild;
                    rchild->red = 0;
                    rchild->right->red = 1;
                }

                // case - 1.3, far child of sibling is red
                // exchange colors of root and rchild
                swap(root->red, rchild->red);
                // left rotate root
                root = rotate_left(root);
                // make right child black
                root->right->red = 0;
                return (ll)root;
            }

            // case - 2, sibling is red (tough)
            // swap colors of root and sibling
            swap(root->red, rchild->red);
            // left rotate
            root = rotate_left(root);
            // reapply cases for the left child, and pass on as if it came from left
            res = _erase(key, root->left, 1, 1);
            root->left = (res == -1) ? nullptr : (Node*)abs(res);
            return (res < 0) ? -(ll)root : (ll)root;
        }
        
        // if not from_left, mirror of above cases
        Node* lchild = root->left;
        if (!lchild->red)   {
            if ((!lchild->right || !lchild->right->red) && (!lchild->left || !lchild->left->red))   {
                // make sibling red
                lchild->red = 1;
                if (root->red)  {
                    // make root black
                    root->red = 0;
                    return (ll)root;    // problem solved
                }
                // root is black, double black problem still exists
                return -(ll)root;
            }

            // case - 1.2, near child of sibling is red
            if (lchild->right && lchild->right->red) {
                // perform left rotation, and exchange colors
                lchild = rotate_left(lchild);
                root->left = lchild;
                lchild->red = 0;
                lchild->left->red = 1;
            }

            // case - 1.3, far child of sibling is red
            // exchange colors of root and lchild
            swap(root->red, lchild->red);
            // right rotate root
            root = rotate_right(root);
            // make left child black
            root->left->red = 0;
            return (ll)root;
        }

        // case - 2, sibling is red (tough)
        // swap colors of root and sibling
        swap(root->red, lchild->red);
        // right rotate
        root = rotate_right(root);
        // reapply cases for the left child, and pass on as if it came from left
        res = _erase(key, root->right, 1, 0);
        root->right = (res == -1) ? nullptr : (Node*)abs(res);
        return (res < 0) ? -(ll)root : (ll)root;
    }

    // returns a node address, and if it needs to be rotated or not
    ll _insert(Node* root, int &key, bool is_left_child=0)    {
        ++root->count;
        // find appropriate place for the key
        if (key < root->val)    {
            if (root->left) {
                ll address = _insert(root->left, key, 1);
                Node *new_root = (Node*)abs(address);
                bool rotate = address<0;
                root->left = new_root;
                if (rotate) {
                    // as it came from the left child, a right rotation must be done
                    root = rotate_right(root);
                    // change colors too
                    // now root must be red and left child must be red as well
                    // make left child black too, as is the right
                    root->left->red = 0;
                }
            }
            else  {  // make new node
                root->left = new Node(key, 1);
                ++tree_size;
            }
        }
        else if (key > root->val)   {
            if (root->right)    {
                ll address = _insert(root->right, key, 0);
                Node *new_root = (Node*)abs(address);
                bool rotate = address<0;
                root->right = new_root;
                if (rotate) {
                    root = rotate_left(root);
                    root->right->red = 0;
                }
            }
            else    {
                root->right = new Node(key, 1);
                ++tree_size;
            }
        }
        else    // key already exists
            return (ll)root;

        // check for problems

        // if both children exist and are red, then this node must be black
        if (root->left && root->right && root->left->red && root->right->red) {
            // change the children and parent colors
            root->red = 1;
            root->left->red = 0;
            root->right->red = 0;
            return (ll)root;
        }
 
        // now, if root is black, then no problem
        if (!root->red)
            return (ll)root;
        
        // check for double red problem in the left child
        if (root->left && root->left->red)  {
            // if root is a left child, then no internal rotation required
            if (is_left_child)
                return -(ll)root;
            // otherwise, internal rotation to the right
            return -(ll)rotate_right(root);
        }
        // check for double red in the right child
        else if (root->right && root->right->red)   {
            if (!is_left_child)
                return -(ll)root;
            return -(ll)rotate_left(root);
        }
        // no rotations required
        return (ll)root;
    }

    // performs right rotation on given root, and returns new root
    Node* rotate_right(Node *root)   {
        Node *left_child = root->left;
        Node *cright = left_child->right;
        root->left = cright;
        left_child->right = root;
        int total_count = root->count;
        root->count += (cright ? cright->count : 0) - left_child->count;
        left_child->count = total_count;
        return left_child;
    }

    // performs left rotation on given root, and returns new root
    Node* rotate_left(Node *root)   {
        Node* right_child = root->right;
        Node *cleft = right_child->left;
        root->right = cleft;
        right_child->left = root;
        int total_count = root->count;
        root->count += (cleft ? cleft->count : 0) - right_child->count;
        right_child->count = total_count;
        return right_child;
    }

    Node* _find(int &key, Node* root)  {
        if (!root)
            return 0;
        if (root->val == key)
            return root;
        if (key < root->val)
            return _find(key, root->left);
        return _find(key, root->right);
    }

    int get_black_height(Node* root)  {
        if (!root)
            return 0;
        return get_black_height(root->left) + !root->red;
    }
    
    bool _check_height(Node* root, int &h, int height)    {
        if (!root)  {
            return height == h;
        }
        return _check_height(root->left, h, height + !root->red) &&
               _check_height(root->right, h, height + !root->red);
    }

    bool _check_red(Node* root) {
        if (!root)
            return 1;
        if (root->red)  {
            if (root->left && root->left->red)
                return 0;
            if (root->right && root->right->red)
                return 0;
        }
        return _check_red(root->left) && _check_red(root->right);
    }

    void _inorder(Node* root, int *ar, int &i)   {
        if (!root)
            return;
        _inorder(root->left, ar, i);
        ar[i++] = root->val;
        _inorder(root->right, ar, i);
    }

    void _clear(Node *root)   {
        if (!root)
            return;
        _clear(root->left);
        _clear(root->right);
        delete root;
    }

    void _print_inorder(Node *root) {
        if (!root)  return;
        _print_inorder(root->left);
        printf("%d ", root->val);
        _print_inorder(root->right);
    }

    int _check_counts(Node *root)  {
        if (!root)
            return 0;
        int lcnt = _check_counts(root->left);
        int rcnt = _check_counts(root->right);
        if (lcnt < 0 || rcnt < 0 || lcnt+rcnt+1 != root->count)
            return -1;
        return root->count;
    }

public:
    rb_tree() : root(nullptr), tree_size(0)  {}

    ~rb_tree()  {
        clear();
    }

    // Deletes all the elements in the tree
    void clear()    {
        _clear(root);
        root = nullptr;
        tree_size = 0;
    }

    // Returns the number of elements present in the tree
    int size()  {
        return tree_size;
    }

    // Inserts a key
    void insert(int key)    {
        if (!root)  {
            root = new Node(key, 0);
            ++tree_size;
            return;
        }
        // find key
        Node *node = _find(key, root);
        if (node)  return;
        
        ll address = _insert(root, key);
        root = (Node*)address;
        if (root->red)
            root->red = 0;
    }

    // Erases a key if present
    void erase(int key) {
        // find if key exists
        Node* node = _find(key, root), *key_node = node;
        if (!node)
            return;
        --tree_size;
        // find the inorder predecessor or successor
        // we'll find the predecessor
        if (node->left) {
            node = node->left;
            while (node->right)
                node = node->right;
        }
        int predecessor = node->val;
        ll res = _erase(predecessor, root);
        root = (res == -1) ? nullptr : (Node*)abs(res);
        if (root && root->red)
            root->red = 0;
        // replace node if it still exists
        if (predecessor != key)
            key_node->val = predecessor;
    }

    // Returns true if key is found in the tree, else false
    bool find(int key)  {
        return _find(key, root);
    }

    // Verifies that there are no double red nodes, returns 1 if verified
    bool check_red()    {
        return _check_red(root);
    }

    // Verifies that the black height of all leaf nodes is same, and returns the height. If not, returns -1
    int check_height()    {
        int h = get_black_height(root);
        bool b = _check_height(root, h, 0);
        if (b)
            return h;
        else
            return -1;
    }

    // Verifies that each subtree contains the correct number of keys
    bool check_counts() {
        return _check_counts(root) >= 0;
    }

    // Stores the inorder traversal of the tree in the array passed
    void inorder(int *ar)  {
        int i = 0;
        _inorder(root, ar, i);
    }

    void _print()   {
        _print_inorder(root);
        printf("\n");
    }

    void level_order()  {
        if (!root)  return;
        queue<Node*> q;
        q.push(root);
        q.push(nullptr);
        while (q.size() > 1)    {
            Node *node = q.front();
            q.pop();
            if (!node)  {
                q.push(node);
                cout << endl;
                continue;
            }
            printf("({%d: %d, %c}: [%d, %d])\t", node->val, node->count, (node->red ? 'r' : 'b'), (node->left ? node->left->val : -1), (node->right ? node->right->val : -1));
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        printf("\n");
    }
};


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
    rb_tree t;
    return 1;
}

bool test2(int t_no)    {
    // constructor and destructor test 2
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.clear();
    return 1;
}

bool test3(int t_no)    {
    // insertion test 1
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.clear();
    t.insert(5);
    return 1;
}

bool test4(int t_no)    {
    // insertion and deletion test
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.insert(5);
    t.erase(5);
    return 1;
}

bool test5(int t_no)    {
    // insertion and deletion test
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.insert(5);
    t.erase(5);
    t.clear();
    return 1;
}

bool test6(int t_no)    {
    // deletion test
    printf("Running test %d: ", t_no);
    rb_tree t;
    t.erase(5);
    return 1;
}

bool test7(int t_no)    {
    // heavy insertion and deletion test
    printf("Running test %d: ", t_no);
    vector<int> test;
    for (int i=0; i<10000; ++i)
        test.push_back(rand() % rand());
    // new set
    set<int> s;
    // new rb_tree
    rb_tree t;

    int n = test.size();

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
    return 1;
}

void run_tests()    {
    typedef bool (*BoolTests) (int t);
    BoolTests tests[] = {test1, test2, test3, test4, test5, test6, test7};
    int n = sizeof(tests) / sizeof(tests[0]);
    for (int i=0; i<n; ++i) {
        cout << (tests[i](i+1) ? "Passed\n" : "Failed\n");
    }
}

int main()  {
    run_tests();
    return 0;
}
