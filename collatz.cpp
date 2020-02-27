#include <iostream>

using namespace std;


struct node {
        unsigned int value;
        int index;
        char color;
        node * parent;
        node * left;
        node * right;
        node * next;
};

//rotacje, fix - upy, usuwanie, dodawanie itp.

void LL(node * & root, node * x, node * & W);
void RR(node * & root, node * x, node * & W);
node * treeInsert(node * & root, unsigned int v, int index, node * & W);
void  RBInsert(node * & root, unsigned int v, int index, node * & W);
//successor - potrzebny do usuwania
node * successor(node * n, node * & W);
//RBDelete
node * min(node * n, node * & W);

node * RBDelete(node * & root, node *z, node * & W);
//fix-up moze osobno, moze wpleciony w delete'a
void RBDeleteFixup(node *& root, node *x, node * & W);
char getColor(node * n);
node * findMin(node * & root, node * & W);
node * findMax(node * & root, node * & W);
unsigned int collatz(node * n, unsigned int t[]);
void freeMemory(node *& root, node *& W);
void addToQueue(node *&root, node * n, node * x, node *& W);


int main()
{

        node * W = (node*)malloc(sizeof(node));
        W->color = 'B';
        W->index = 0;
        W->value = 0;
        W->left = W->parent = W->right = W->next = W;
        node * root = W;
        int n;
        int treeSize = 0;
        scanf("%d", &n);
        unsigned int * tab = (unsigned int *)malloc(n * sizeof(unsigned int));
        for (int i = 0; i < n; i++) {
                scanf(" %u", &tab[i]);
                if (tab[i] > 1) {
                        RBInsert(root, tab[i], i, W);
                        treeSize++;
                }
        }
        int numberOfCommands = 0;
        scanf(" %d", &numberOfCommands);
        int counter = 0;
        char type = 'x';
        unsigned int newValue = 0;
        int indeks;
        while (numberOfCommands-- && treeSize > 0) {
                scanf(" %d", &counter);
                scanf(" %c", &type);
                while (counter-- && treeSize > 0) {
                        if (type == 's') {
                                node * n = findMin(root, W);
                                indeks = n->index;
                                newValue = collatz(n, tab);
                                if (n->value > newValue && newValue >1 && n->next ==W) {
                                        n->value = newValue;
                                }
                                else {
                                        RBDelete(root, n, W);
                                        free(n);
                                        treeSize--;
                                        if (newValue > 1) {
                                                RBInsert(root, newValue, indeks, W);
                                                treeSize++;
                                        }
                                }

                        }
                        if (type == 'l') {
                                node * n = findMax(root, W);
                                indeks = n->index;
                                newValue = collatz(n, tab);
                                if (n->value < newValue && newValue >1 && n->next ==W) {
                                        n->value = newValue;
                                }
                                else {
                                        RBDelete(root, n, W);
                                        free(n);
                                        treeSize--;
                                        if (newValue > 1) {
                                                RBInsert(root, newValue, indeks, W);
                                                treeSize++;
                                        }
                                }
                        }
                }
        }

        for (int i = 0; i < n; i++) {
                if (tab[i] != 0) {
                        printf("%u ", tab[i]);
                }
                else {
                        printf("m ");
                }
        }
        free(tab);
        freeMemory(root, W);
        free(W);

        return 0;
}

void LL(node *& root, node * x, node * & W)
{
        node *y = x->right;
        x->right = y->left;
        if (y->left != W) {
                y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == W) {
                root = y;
        }
        else if (x == x->parent->left) {
                x->parent->left = y;
        }
        else {
                x->parent->right = y;
        }
        y->left = x;
        x->parent = y;

}

void RR(node *& root, node * x, node * & W)
{
        node *y = x->left;
        x->left = y->right;
        if (y->right != W) {
                y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == W) {
                root = y;
        }
        else if (x == x->parent->left) {
                x->parent->left = y;
        }
        else {
                x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
}


node * treeInsert(node *& root, unsigned int v, int index, node * & W)
{
        node * n = (node*)malloc(sizeof(node));
        n->left = n->right = n->parent = n->next = W;
        n->index = index;
        n->value = v;
        n->color = 'R';
        node * y = W;
        node * x = root;
        while (x != W) {
                if (x->value == n->value) {
                        addToQueue(root, n, x, W);
                        return root;
                }
                y = x;
                if (n->value < x->value) {
                        x = x->left;
                }
                else {
                        x = x->right;
                }
        }
        n->parent = y;
        if (y == W) {
                root = n;
        }
        else {
                if (n->value < y->value) {
                        y->left = n;
                }
                else {
                        y->right = n;
                }
        }

        return n;
}

void RBInsert(node *& root, unsigned int v, int index, node * & W)
{
        node * x;
        node * y;
        x = treeInsert(root, v, index, W);
        while (x != root && x->parent->color == 'R') {
                if (x->parent == x->parent->parent->left) { //porzadkowanie dla ojca po lewej
                        y = x->parent->parent->right;
                        if (getColor(y) == 'R') {
                                x->parent->color = 'B';
                                y->color = 'B';
                                x->parent->parent->color = 'R';
                                x = x->parent->parent;
                                continue;
                        }
                        else {
                                if (x == x->parent->right) {
                                        x = x->parent;
                                        LL(root, x, W);
                                }
                                x->parent->color = 'B';
                                x->parent->parent->color = 'R';
                                RR(root, x->parent->parent, W);
                                break;
                        }
                }
                else { // porzadkowanie dla ojca po prawej
                        y = x->parent->parent->left;
                        if (getColor(y) == 'R') {
                                x->parent->color = 'B';
                                y->color = 'B';
                                x->parent->parent->color = 'R';
                                x = x->parent->parent;
                                continue;
                        }
                        else {
                                if (x == x->parent->left) {
                                        x = x->parent;
                                        RR(root, x, W);
                                }
                                x->parent->color = 'B';
                                x->parent->parent->color = 'R';
                                LL(root, x->parent->parent, W);
                                break;
                        }
                }
        }
        root->color = 'B';
}

node * successor(node * n, node * & W)
{
        node * r;
        if (n != W) {
                if (n->right != W) {
                        return min(n->right, W);
                }
                else {
                        r = n->parent;
                        while ((r != W) && (n == r->right)) {
                                n = r;
                                r = r->parent;
                        }
                        return r;
                }
        }
        return W;
}

node * min(node * n, node * & W)
{
        while (n->left != W) {
                n = n->left;
        }
        return n;
}

node * RBDelete(node *& root, node * z, node *& W)
{
        if (z->next != W) {
                node * n = z->next;
                n->color = z->color;
                n->parent = z->parent;
                if (n->parent->left == z) {
                        n->parent->left = n;
                }
                else {
                        n->parent->right = n;
                }
                n->left = z->left;
                n->left->parent = n;
                n->right = z->right;
                n->right->parent = n;
                if (z == root) {
                        root = n;
                }
                return z;
        }
        node * y;
        node *x;
        if (z->left == W || z->right == W) {
                y = z;
        }
        else {
                y = successor(z, W);
        }
        if (y->left != W) {
                x = y->left;
        }
        else {
                x = y->right;
        }

        x->parent = y->parent;

        if (y->parent == W) {
                root = x;
        }
        else if (y == y->parent->left) {
                y->parent->left = x;
        }
        else {
                y->parent->right = x;
        }
        if (y != z) {
                z->index = y->index;
                z->value = y->value;
        }
        if (y->color == 'B') {
                RBDeleteFixup(root, x, W);
        }
        return y;
}

void RBDeleteFixup(node *& root, node * x, node *& W)
{
        node * br;
        while (x != root && x->color == 'B') {
                if (x == x->parent->left) { //tu ustalamy, że x jest lewym synem
                        br = x->parent->right;
                        if (br->color == 'R') { //przypadek 1
                                br->color = 'B';
                                x->parent->color = 'R';
                                LL(root, x->parent, W);
                                br = x->parent->right;
                        }
                        if (br->left->color == 'B' && br->right->color == 'B') { //przypadek 2
                                br->color = 'R';
                                x = x->parent;
                        }
                        else {
                                if (br->right->color == 'B') { //przypadek 3
                                        br->left->color = 'B';
                                        br->color = 'R';
                                        RR(root, br, W);
                                        br = x->parent->right;
                                }
                                //przypadek 4
                                br->color = x->parent->color;
                                x->parent->color = 'B';
                                br->right->color = 'B';
                                LL(root, x->parent, W);
                                x = root;
                        }
                }
                else { // tu dla x będącego prawym synem
                        br = x->parent->left;
                        if (br->color == 'R') { //przypadek 1
                                br->color = 'B';
                                x->parent->color = 'R';
                                RR(root, x->parent, W);
                                br = x->parent->left;
                        }
                        if (br->left->color == 'B' && br->right->color == 'B') { //przypadek 2
                                br->color = 'R';
                                x = x->parent;
                        }
                        else {
                                if (br->left->color == 'B') { //przypadek 3
                                        br->right->color = 'B';
                                        br->color = 'R';
                                        LL(root, br, W);
                                        br = x->parent->left;
                                }
                                //przypadek 4
                                br->color = x->parent->color;
                                x->parent->color = 'B';
                                br->left->color = 'B';
                                RR(root, x->parent, W);
                                x = root;
                        }
                }
        }
        x->color = 'B';
}

char getColor(node * n)
{
        return n->color;
}

node * findMin(node *& root, node *& W)
{
        node * x = root;
        while (x->left != W) {
                x = x->left;
        }
        return x;
}

node * findMax(node *& root, node *& W)
{
        node * x = root;
        while (x->right != W) {
                x = x->right;
        }
        return x;
}

unsigned int collatz(node * n, unsigned int t[])
{
        int index = n->index;
        unsigned int oldValue = n->value;
        unsigned int newValue;
        if (oldValue % 2 == 0) {
                newValue = oldValue / 2;
                t[index] = newValue;
        }
        else {
                if (oldValue > 1431655764) {
                        t[index] = 0;
                        return 0;
                }
                else {
                        newValue = 3 * oldValue + 1;
                        t[index] = newValue;
                }
        }
        return newValue;
}

void freeMemory(node *& root, node *& W)
{
        if (root == W) {
                return;
        }
        if (root->left != W) {
                freeMemory(root->left, W);
        }
        if (root->right != W) {
                freeMemory(root->right, W);
        }
        free(root);
}

void addToQueue(node *& root, node * n, node * x, node *& W)
{
        if (n->index < x->index) { //jeżeli bedzie pierwszy
                if (x == root) {
                        root = n;
                }
                n->parent = x->parent;
                if (n->parent->left == x) {
                        n->parent->left = n;
                }
                else {
                        n->parent->right = n;
                }
                n->color = x->color;
                n->left = x->left;
                n->left->parent = n;
                n->right = x->right;
                n->right->parent = n;
                n->next = x;
                x->left = x->right = x->parent = W;
                return;
        }
        node * tmp = x;
        while(x->index < n-> index){ //tu bedzie ktorys w kolejce
                x = x->next;
                if (x->index > n->index || x == W) {
                        tmp->next = n;
                        n->next = x;
                        return;
                }
                tmp = x;
        }
        return;
}
