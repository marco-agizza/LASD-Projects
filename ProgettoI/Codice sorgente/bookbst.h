#ifndef BIBLIOTECA_BOOKBST_H
#define BIBLIOTECA_BOOKBST_H

#include "utility.h"
#define MAX 100

typedef struct book{
    char title[MAX];    //identificativo univoco di un libro
    char author[MAX];   //autore/i del libro
    int availability;   //indica il numero di copie disponibili del libro
    struct book *right; //puntatore a nodo figlio destro
    struct book *left;  //puntatore a nodo figlio sinistro
}book;

//Popola la biblioteca di 15 titoli in modo che l' albero risulti bilanciato
book *initializeLibrary(void);

/*Si occupa di aggiungere un libro alla raccolta della biblioteca rispettando la relazione d' ordine stabilita sul campo "title"*/
book *addBook(book *newbook, char *title, char *author, int copies);

/*Si occupa di allocare il nuovo libro in memoria*/
book *newBook(char *title, char *author, int copies);

//elimina un nodo dell' albero
void deleteBook(book **tree, char *title);

//dealloca l' albero dalla memoria
void destroyLibrary(book **library);

//ritorna il puntatore a book di un libro con un determinato titolo
book *getBook(book *library, char *title);

//Stampa pre order della biblioteca in ingresso
void preOrderStamp(book *library);

//Stampa post order della biblioteca in ingresso
void postOrderStamp(book *library);

//Stampa in order della biblioteca in ingresso
void inOrderStamp(book *library);

//Controlla che un libro sia disponibile o meno in biblioteca
int checkAvailability(book *library, char *title);

#endif //BIBLIOTECA_BOOKBST_H
