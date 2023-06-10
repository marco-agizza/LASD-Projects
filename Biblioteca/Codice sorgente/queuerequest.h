#ifndef BIBLIOTECA_QUEUEREQUEST_H
#define BIBLIOTECA_QUEUEREQUEST_H

#include "utility.h"
#include "bookbst.h"
#include "studentbst.h"
#include "queueloans.h"

typedef struct request{
    student *studente;
    book *libro;
    struct request *next, *prev;
}request;

//metto in testa alla coda una nuova richiesta
request *insertRequest(request *requests, book *library, student *richiedente, book *libro);

//dealloca dalla memoria la lista della richieste
void destroyRequests(request **request);

//essa viene chiamata quando viene consegnato un libro: se il libro risulta tra i prenotati viene consegnato al primo prenotato ----------------------------------
request *checkDeleteRequest(request *queue, book *libro);

//checkRequest controlla se la richiesta di uno studente per un certo libro è già presente (torna 1 se la richiesta non è già stata fatta)
int checkRequest(request *queue, char *matricola, char *titolo);

void queueStamp(request *queue);

#endif //BIBLIOTECA_QUEUEREQUEST_H
