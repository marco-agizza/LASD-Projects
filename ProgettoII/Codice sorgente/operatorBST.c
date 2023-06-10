#include "operatorBST.h"

operatore *initializeOperators(void){
    FILE *fp=NULL;
    operatore *operatorBST=NULL;
    char password[PWSIZE];
    char id[IDSIZE];

    if((fp= fopen("operators_list.txt", "r"))==NULL){
        printf("ERRORE APERTURA FILE\n");
        exit(1);
    }
    while(!feof(fp)){
        fscanf(fp, "%s %s", id, password);
        operatorBST= insertOperator(operatorBST, id, password);
    }
    fclose(fp);
    return operatorBST;
}

operatore *insertOperator(operatore *operatorBST, char *id_operator, char *password){
    if(operatorBST){
        if(strcmp(operatorBST->id_operator,id_operator)<0){
            operatorBST->right= insertOperator(operatorBST->right, id_operator, password);
        }else if(strcmp(operatorBST->id_operator,id_operator)>0){
            operatorBST->left= insertOperator(operatorBST->left, id_operator, password);
        }
    }else{
        operatorBST= newOperator(id_operator, password);
    }
    return operatorBST;
}

operatore *newOperator(char *id_operator, char *password){
    operatore *newoperator=(operatore*)malloc(sizeof(operatore));
    strcpy(newoperator->id_operator, id_operator);
    strcpy(newoperator->password, password);
    newoperator->left=NULL;
    newoperator->right=NULL;
    return newoperator;
}

operatore *getOperatore(operatore *operatorBST, char *id_operator){
    if(operatorBST){
        if(strcmp(operatorBST->id_operator, id_operator)<0){
            return getOperatore(operatorBST->right, id_operator);
        }else if(strcmp(operatorBST->id_operator, id_operator)>0){
            return getOperatore(operatorBST->left, id_operator);
        }else{
            return operatorBST;
        }
    }else{
        return NULL;
    }
}

int checkOperatore(operatore *operatorBST, char *id_operator, char *password){
    operatore *user= getOperatore(operatorBST, id_operator);
    if(user){
        if(strcmp(user->password, password)==0){
            return 1;
        }else{
            printf(RED"Password errata!\n"RESET);
        }
    }else{
        printf(RED"L'operatore inserito non risulta registrato\n"RESET);
    }
    return 0;
}

void printOperators(operatore *operatorBST){
    if(operatorBST) {
        printOperators(operatorBST->left);
        printf("Codice operatore: %s\n", operatorBST->id_operator);
        printOperators(operatorBST->right);
    }
}