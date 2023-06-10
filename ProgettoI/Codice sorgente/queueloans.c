#include "queueloans.h"

prestiti *insertLoan(prestiti *queue, book *libro){
    prestiti *newloan=(prestiti*)malloc(sizeof(prestiti));
    now(&newloan->giorno,&newloan->mese,&newloan->anno,&newloan->ora,&newloan->minuti);
    newloan->libro=libro;
    newloan->next=queue;
    newloan->prev=NULL;
    if(queue)
        queue->prev=newloan;
    libro->availability-=1;

    return newloan;
}

prestiti *deleteLoan(prestiti *queue, book *libro, int *err){
    if(queue){
        if(queue->next){
            queue->next= deleteLoan(queue->next, libro,err);
        }else{
            if(strcmp(queue->libro->title,libro->title)==0){
                //il libro che lo studente vuole togliersi è quello che ha da più tempo: OK
                free(queue);
                libro->availability+=1;
                return NULL;
            }else{
                printf(YELLOW"\tattenzione"RESET": impossibile restituire %s; lo studente deve consegnare prima %s\n", libro->title, queue->libro->title);
                *err=1;
            }
        }
    }else{
        *err=1;
    }
    return queue;
}

prestiti *forcedDeleteLoan(prestiti *queue, book *libro){
    if(queue){
        if(strcmp(queue->libro->title,libro->title)==0){
            //trovato il libro che si vuole rimuovere
            prestiti *temp=queue;
            queue=queue->next;
            if(queue)
                queue->prev=temp->prev;
            libro->availability+=1;
            free(temp);
        }else{
            queue->next=forcedDeleteLoan(queue->next,libro);
        }
    }
    return queue;
}

int timeExpired(prestiti *queue, char *title){
    if(queue){
        if(strcmp(queue->libro->title,title)==0){
            //trovato il libro tra i posseduti;
            int giorno, mese, anno, ora, minuti;
            now(&giorno,&mese,&anno,&ora,&minuti);
            if(giorno==queue->giorno && mese==queue->mese && anno==queue->anno){
                if(ora==queue->ora){
                    return 0;
                }else{
                    if(ora==(queue->ora)+1){
                        if(minuti<=queue->minuti){
                            return 0;
                        }else{
                            return 1;
                        }
                    }else{
                        return 1;
                    }
                }
            }else{
                return 1;
            }
        }else{
            return timeExpired(queue->next, title);
        }
    }
    return 0;
}

int checkLoans(prestiti *queue, char *title){
    if(queue){
        if(strcmp(queue->libro->title,title)==0){
            return 0;
        }else{
            return checkLoans(queue->next, title);
        }
    }
    return 1;
}

void destroyLoans(prestiti **loans){
    if(*loans){
        prestiti *temp=NULL;
        temp=*loans;
        loans=&(*loans)->next;
        free(temp);
        destroyLoans(loans);
    }
}

void bookStamp(prestiti *queue){
    if(queue){
        printf("\t %s di %s [%d/%d/%d %d:%d]\n", queue->libro->title, queue->libro->author, queue->giorno, queue->mese, queue->anno, queue->ora, queue->minuti);
        bookStamp(queue->next);
    }
}