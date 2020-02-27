#include <iostream>
#include <stdlib.h>
 
 
struct node {
    node * next;
    char value;
};
node* push(node * n, char c); //wstawienie nowego wezla na poczatek
node* pop(node *n); //usuniecie pierwszego wezla
char read(node*); //odczytanie wartosci
 
using namespace std;
 
int main()
{
    node a;
    node* wsk = &a;
    wsk->next = NULL;
    wsk->value = 'b';
   
    char *ciag;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) { //tyle razy ile podal uzytkownik wczytujemy ciag onp
        ciag = (char*)malloc(500 * sizeof(char));
        cin >> ciag;
        int j = 0;
        while (ciag[j] != '\0') { //tutaj czytamy znak po znaku i liczymy
            char znak = ciag[j];
                if (znak == 't' || znak == 'f') {
                wsk = push(wsk, znak);
                }
                else {
                    if (znak == 'N') {
                        char x = read(wsk);
                        wsk = pop(wsk);
                        if (x == 'f') {
                            wsk = push(wsk, 't');
                        }
                        else {
                            wsk = push(wsk, 'f');
                        }
                    }
                    if (znak == 'A') {
                        char x = read(wsk);
                        wsk = pop(wsk);
                        char y = read(wsk);
                        wsk = pop(wsk);
                        if (x == 't' || y == 't') {
                            wsk = push(wsk, 't');
                        }
                        else {
                            wsk = push(wsk, 'f');
                        }
 
                    }
                    if (znak == 'K') {
                        char x = read(wsk);
                        wsk = pop(wsk);
                        char y = read(wsk);
                        wsk = pop(wsk);
                        if (x == 'f' || y == 'f') {
                            wsk = push(wsk, 'f');
                        }
                        else {
                            wsk = push(wsk, 't');
                        }
                    }
                }
 
            j++;
        }
        cout << read(wsk);
        free(ciag);
        wsk->next = NULL;
        cout << endl;
    }
   
    return 0;
}
 
node* push(node * n, char c)
{
    node * nowy;
    nowy = (node*)malloc(sizeof(node));
    nowy->value = c;
    nowy->next = n;
    n = nowy;
    return nowy;
}
 
 
node * pop(node * n) {
    node *x = n;
    n = n->next;
    //free(x);
    return n;
}
 
char read(node *n) {
    return n->value;
}
