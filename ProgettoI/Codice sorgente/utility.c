#include "utility.h"

void getStr(char *str, int len){
    assert(str!=NULL);
    int i;
    char c;
    // scanf("%*[^\n]");scanf("%*c"); //pulisco il buffer
    for(i=0;(i<len)&&((c=getchar()) != '\n') && (c!=EOF);++i){
        str[i]=c;
    }
    str[i]='\0';
}

void now(int *giorno, int* mese, int *anno, int *ora, int* minuti){
    struct tm *readable=NULL;
    time_t moment;
    time(&moment);
    readable=localtime(&moment);
    *giorno=readable->tm_mday;
    *mese=readable->tm_mon+1;
    *anno=readable->tm_year+1900;
    *ora=readable->tm_hour;
    *minuti=readable->tm_min;
}

void mypause(void){
    printf("Premi invio per continuare...");
    fflush(stdin);
    fflush(stdout);
    getchar();
}

void takeAnswer(char *answer){
    do{
        fflush(stdin);
        scanf("%c",answer);
        if(*answer!='S' && *answer!='N')
            printf(RED"Attenzione"RESET", inserire ''S'' o ''N'': ");
        fflush(stdin);
    }while (*answer!='S' && *answer!='N');
}

void takeSaneString(char *string,int size){
    do{
        fflush(stdin);
        getStr(string, size);
        if(string[0]=='\0')
            printf(RED"errore"RESET", non sono ammessi campi vuoti: ");
        fflush(stdin);
    }while(string[0]=='\0');
}

void takeSaneIDStudent(char *matricola, int size){
    do{
        fflush(stdin);
        getStr(matricola, size);
        if(strlen(matricola)!=9 && strcmp(matricola,"-1")!=0)
            printf(RED"errore"RESET", le matricole sono composte da 9 caratteri"GRAY" [-1 per tornare al menu]"RESET": ");
        fflush(stdin);
    }while(strlen(matricola)!=9 && strcmp(matricola,"-1")!=0);
}