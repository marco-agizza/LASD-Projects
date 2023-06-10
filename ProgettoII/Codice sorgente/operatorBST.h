#ifndef PROGETTOII_OPERATORBST_H
#define PROGETTOII_OPERATORBST_H

#include "utility.h"

typedef struct operatore{
    char id_operator[9],
            password[13];
    struct operatore *left,
            *right;
}operatore;

//legge il file in cui sono registrati gli operatori della compagnia aerea
operatore *initializeOperators(void);

//inserisce all'interno dell'albero l'operatore in ingresso
operatore *insertOperator(operatore *operatorBST, char *id_operator, char *password);

//alloca l'operatore in memoria
operatore *newOperator(char *id_operator, char *password);

//ritorna il puntatore a nodo dell'operatore identificato dalla combinazione di id e password in ingresso
operatore *getOperatore(operatore *operatorBST, char *id_operator);

//controlla se le credenziali inserite sono corrette
int checkOperatore(operatore *operatorBST, char *id_operator, char *password);

//stampa l'elenco di tutti gli operatori registrati
void printOperators(operatore *operatorBST);


#endif //PROGETTOII_OPERATORBST_H