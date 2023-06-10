#include "studentbst.h"

student *addStudent(student *node, char *nome, char *cognome, char *matricola){
    if(node){
        if(strcmp(node->matricola,matricola)<0){
            node->right=addStudent(node->right, nome, cognome, matricola);
        }else if(strcmp(node->matricola, matricola)>0){
            node->left=addStudent(node->left, nome, cognome, matricola);
        }else{
            printf(RED"error"RESET": lo studente e' gia' stato registrato.\n");
        }
    }else{
        node=newStudent(nome, cognome, matricola);
    }
    return node;
}
student *newStudent(char *nome, char *cognome, char *matricola){
    student *newstudent=(student*)malloc(sizeof(student));
    strcpy(newstudent->nome, nome);
    strcpy(newstudent->cognome, cognome);
    strcpy(newstudent->matricola, matricola);
    newstudent->right=NULL;
    newstudent->left=NULL;
    newstudent->borrowedbooks=NULL;
    return newstudent;
}

void destroyStudents(student **students){
    if(*students){
        destroyStudents(&(*students)->left);
        destroyStudents(&(*students)->right);
        destroyLoans(&(*students)->borrowedbooks);
        free(*students);
        *students=NULL;
    }
}

void studentStamp(student *node){
    if(node){
        studentStamp(node->left);
        printf("\t\t      %s %s %s\n",node->nome,node->cognome,node->matricola);
        studentStamp(node->right);
    }
}

student *getStudent(student *node, char *matricola){
    if(node){
        if(strcmp(node->matricola,matricola)<0){
            return getStudent(node->right, matricola);
        }else if(strcmp(node->matricola, matricola)>0){
            return getStudent(node->left, matricola);
        }else{
            return node;
        }
    }else{
        return NULL;
    }
}

void rentBook(student **node, book *libro){
    if(*node){
        if(checkLoans((*node)->borrowedbooks, libro->title)==1){
            (*node)->borrowedbooks=insertLoan((*node)->borrowedbooks, libro);
            printf(GREEN"\tNoleggio registrato con successo a %s\n"RESET, (*node)->matricola);
        }else{
            printf(RED"\terror"RESET": lo studente gia' è in possesso del libro selezionato\n");
        }
    }
}

void returnBook(student **node,book *libro, int *err){
    *err=0;
    if(*node){
        if(checkLoans((*node)->borrowedbooks, libro->title)==0){
            //il libro è tra quelli noleggiati dallo studente
            (*node)->borrowedbooks=deleteLoan((*node)->borrowedbooks, libro, err);
        }else{
            printf(RED"\terror"RESET": non risulta che allo studente %s sia stato dato il libro %s.\n", (*node)->matricola, libro->title);
            *err=1;
        }
    }
}

void forcedReturnBook(student **node,book *libro, int *err){
    *err=0;
    if(*node){
        if(checkLoans((*node)->borrowedbooks, libro->title)==0){
            //il ibro effettivamente è stato prestato allo studente *node
            if(timeExpired((*node)->borrowedbooks, libro->title)==1) {
                //è trascorsa almeno un'ora da quando il libro è stato consegnato allo studente
                (*node)->borrowedbooks = forcedDeleteLoan((*node)->borrowedbooks, libro);
            }else{
                printf(YELLOW"\tattenzione"RESET": deve essere passata almeno un'ora da quando e' stato noleggiato il libro\n");
                *err=1;
            }
        }else{
            printf(RED"\terror"RESET": non risulta che allo studente %s sia stato dato il libro %s.\n", (*node)->matricola, libro->title);
            *err=1;
        }
    }
}

void stampaLibriPosseduti(student *node){
    bookStamp(node->borrowedbooks);
}