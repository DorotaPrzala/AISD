#include <iostream>
#include <string>

using namespace std;

struct node {
        string trans;
        node * array[26];
};

node * createNode();
void addTranslation(node * root, const string word, const string translation);
void findTranslation(node * root, const string word);
node * findNode(node * root, const string prefix);
void printTranslation(node * root);

int main()
{
        node * root = createNode();

        char sign;
        while (cin >> sign) {
                if (sign == '+') {
                        string word;
                        string translation;
                        cin >> word;
                        cin >> translation;
                        addTranslation(root, word, translation);
                }
                if (sign == '?') {
                        string word;
                        cin >> word;
                        findTranslation(root, word);

                }
                if (sign == '*') {
                        string prefix;
                        cin >> prefix;
                        node * p = findNode(root, prefix);
                        if (p != NULL) {
                                printTranslation(p);
                        }
                }
        }
        return 0;
}

node * createNode()
{
        node * n = new node;
        n->trans = "";
        for (int i = 0; i < 26; i++) {
                n->array[i] = NULL;
        }
        return n;
}

void addTranslation(node * root, const string word, const string translation)
{
        int length = word.length();
        for (int i = 0; i < length; i++) {
                int direction = static_cast<int>(word[i]) - static_cast<int>('a');
                if (root->array[direction] == NULL) {
                        root->array[direction] = createNode();
                }
                root = root->array[direction];
        }
        root->trans = translation;
}

void findTranslation(node * root, const string word)
{
        int length = word.length();
        for (int i = 0; i < length; i++) {
                int direction = static_cast<int>(word[i]) - static_cast<int>('a');
                if (root->array[direction] == NULL) {
                        cout << "-" << endl;
                        return;
                }
                root = root->array[direction];
        }

        if (root->trans != "") {
                cout << root->trans << endl;
        }
        else {
                cout << "-" << endl;
        }
}

node * findNode(node * root, const string prefix)
{
        int length = prefix.length();
        for (int i = 0; i < length; i++) {
                int direction = static_cast<int>(prefix[i]) - static_cast<int>('a');
                if (root->array[direction] == NULL) {
                        cout << "-" << endl;
                        return NULL;
                }
                else {
                        root = root->array[direction];
                }
        }
        return root;
}

void printTranslation(node * root)
{
        if (root->trans != "") {
                cout << root->trans << endl;
        }

        for (int i = 0; i < 26; i++) {
                if (root->array[i] != NULL) {
                        printTranslation(root->array[i]);
                }
        }
}
