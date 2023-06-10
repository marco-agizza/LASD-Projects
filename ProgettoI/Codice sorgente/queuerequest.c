#include "queuerequest.h"

request *insertRequest(request *requests, book *library, student *richiedente, book *libro){
    if(requests){
        if(requests->next){
            requests->next=insertRequest(requests->next, library, richiedente, libro);
        }else{
            request *newrequest=(request*)malloc(sizeof(request));
            newrequest->libro=libro;
            newrequest->studente=richiedente;
            newrequest->prev=requests;
            newrequest->next=NULL;
            requests->next=newrequest;
        }
    }else{
        request *newrequest=(request*)malloc(sizeof(request));
        newrequest->libro=libro;
        newrequest->studente=richiedente;
        newrequest->next=NULL;
        newrequest->prev=NULL;
        return newrequest;
    }
    return requests;
}

void destroyRequests(request **requests){
    if(*requests){
        request  *temp=NULL;
        temp=*requests;
        requests=&(*requests)->next;
        free(temp);
        destroyRequests(requests);
    }
}

request *checkDeleteRequest(request *queue, book *libro){
    if(queue){
        if(strcmp(queue->libro->title,libro->title)==0){
            //il libro è tra quelli richiesti
            rentBook(&queue->studente, libro);
            request *temp=queue;
            if(queue->next)
                queue->next->prev=queue->prev;
            queue=queue->next;
            free(temp);
            temp=NULL;
        }else{
            queue->next=checkDeleteRequest(queue->next, libro);
        }
    }
    return queue;
}

int checkRequest(request *queue, char *matricola, char *titolo){
    if(queue){
        if(strcmp(queue->studente->matricola,matricola)==0 && strcmp(queue->libro->title, titolo)==0){
            printf("Lo studente ha gia' richiesto questo libro.\n");
            return 0;
        }else{
            return checkRequest(queue->next, matricola, titolo);
        }
    }
    return 1;
}

void queueStamp(request *queue){
    if(queue){
        printf("%s %s (%s): %s di %s\n", queue->studente->nome, queue->studente->cognome, queue->studente->matricola, queue->libro->title, queue->libro->author);
        queueStamp(queue->next);
    }
}

