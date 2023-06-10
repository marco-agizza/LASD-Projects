#ifndef BIBLIOTECA_STUDENTBST_H
#define BIBLIOTECA_STUDENTBST_H

#include <stdio.h>
#include "bookbst.h"
#include "queueloans.h"

typedef struct student{
    char nome[50];
    char cognome[50];
    char matricola[10];
    prestiti *borrowedbooks; //coda di libri presi in prestito
    struct student *left, *right;
}student;

/*Si occupa di aggiungere uno nuovo studente all' albero rispettando la relazione d' ordine sul campo "matricola"*/
student *addStudent(student *node, char *nome, char *cognome, char *matricola);

//Si occupa di allocare il nuovo studente in memoria
student *newStudent(char *nome, char *cognome, char *matricola);

//Dealloca l' albero che colleziona gli studenti dalla memoria
void destroyStudents(student **students); //deve usare una destroyLoans per ogni nodo...

//Stampa in order degli studenti registrati
void studentStamp(student *node);

//Ritorna il puntatore a nodo dello studente con una certa matricola
student *getStudent(student *node, char *matricola);

//Permette di aggiungere un libro alla coda dei libri noleggiati dello studente in ingresso
void rentBook(student **node, book *libro);

//Permette di eliminare un libro dalla coda dei libri noleggiati dallo studenti in ingresso
void returnBook(student **node,book *libro, int *err);

//Permette di eliminare un libro senza rispettare l' ordine di ingresso FIFO se è passata almeno un'ora da quando questo è stato noleggiato dallo studente
void forcedReturnBook(student **node,book *libro, int *err);

//Permette di stampare a schermo i libri posseduti dallo studente in ingresso
void stampaLibriPosseduti(student *node);

#endif //BIBLIOTECA_STUDENTBST_H
