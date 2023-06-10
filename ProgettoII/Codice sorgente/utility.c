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

void modifyString(char *str1, char *str2){
    assert(str1!=NULL);
    assert(str2!=NULL);
    char c;
    int i=0;

    while(str2[i]!='\0' && str2[i]!='\n'){
        str1[i]=str2[i];
        i++;
    }
    str1[i]='\0';
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
        if(*answer!='s' && *answer!='n')
            printf(YELLOW"warning"RESET": inserire \"s\" o \"n\": ");
        fflush(stdin);
    }while (*answer!='s' && *answer!= 'n');
}

void takeAnswerMenu(char *answer){
    do{
        fflush(stdin);
        scanf("%c",answer);
        if(*answer!='s' && *answer!='n')
            printf(YELLOW"\nwarning"RESET": inserire \"s\", \"n\" o \"0\": ");
        fflush(stdin);
    }while (*answer!='s' && *answer!= 'n' && *answer!='0');
}

void takeSaneString(char *string,int size){
    do{
        fflush(stdin);
        getStr(string, size);
        if(string[0]=='\0')
            printf(RED"error"RESET", non sono ammessi campi vuoti: ");
        fflush(stdin);
    }while(string[0]=='\0');
}

int equalsDate(struct tm *data1, struct tm *data2){
    if(data1->tm_mday==data2->tm_mday){
        if(data1->tm_mon==data2->tm_mon){
            if(data1->tm_year==data2->tm_year)
                return 1;
        }
    }
    return 0;
}

int equalsTime(struct tm *orario1, struct tm *orario2){
    if(orario1->tm_hour==orario2->tm_hour){
        if(orario1->tm_min==orario2->tm_min)
            return 1;
    }
    return 0;
}

void insertPassword(char *pword, int size){
    int ch;
    int i = 0;

    fflush(stdout);
    while ((ch = getch()) != '\n' && ch != '\r' && i < size - 1){
        if (ch == '\b' && i > 0){
            printf("\b \b");
            fflush(stdout);
            i--;
            pword[i] = '\0';
        } else if (isalnum(ch)){
            putchar('*');
            pword[i++] = (char)ch;
        }
    }
    pword[i] = '\0';
}

void twirlybar(void) {
    static clock_t last = 0;
    static const char *dir = "|/-\\|/-\\|";
    static size_t angle = 0;
    clock_t now;

    now = clock();
    if((now - last) / (CLOCKS_PER_SEC / 10.0) >= 0.5) {
        if(++angle == sizeof(dir)/sizeof(*dir)) angle = 0;
        last = now;
    }
    putchar('\b');
    putchar(dir[angle]);
    fflush(stdout);
}

void caricamento(char *message, int reps){

    printf("%s", message);
    for(int i=0; i<reps;i++){
        twirlybar();
        Sleep(300);
    }
}

int checkOccurrence(int *array, int size, int value) {
    if(array[value]==2)
        return 1;
    return 0;
}



void clearScreen(void){
    for (int n = 0; n < 10; n++)
        printf( "\n\n\n\n\n\n\n\n\n\n" );
}