#ifndef BIBLIOTECA_QUEUELOANS_H
#define BIBLIOTECA_QUEUELOANS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bookbst.h"

typedef struct prestiti{
    book *libro;
    int giorno, mese, anno, ora, minuti;
    struct prestiti *next, *prev;
}prestiti;

//inserisce un libro nella coda dei libri presi in prestito e decrementa availability del libro
prestiti *insertLoan(prestiti *queue, book *libro);

//la delete di un libro dalla coda dei libri presi in prestito
prestiti *deleteLoan(prestiti *queue, book *libro, int *err);

//la delete di un libro dalla coda dei libri presi in prestito dallo studente in ingresso senza rispettare la regola FIFO delle queue
prestiti *forcedDeleteLoan(prestiti *queue, book *libro);

//ritorna 1 se è passata almeno 1h da quando è stato consegnato il libro
int timeExpired(prestiti *queue, char *title);

//ritorna 1 se il libro  non è presente
int checkLoans(prestiti *queue, char *title);

//dealloca la lista dalla memoria
void destroyLoans(prestiti **loans);

void bookStamp(prestiti *queue);

#endif //BIBLIOTECA_QUEUELOANS_H
