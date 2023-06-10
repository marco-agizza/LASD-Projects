#include "clientBST.h"

cliente *initClients(void){
    struct tm *data_nascita;
    char temp1[MAXNAME], temp2[MAXNAME];
    char nome[MAXNAME], cognome[MAXNAME], e_mail[MAXNAME], password[MAXNAME];
    FILE *fp=NULL;

    cliente *tree_client=NULL;

    if((fp= fopen("clients_list.txt", "r"))==NULL){
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }

    while(!feof(fp)){
        data_nascita=malloc(sizeof(struct tm));
        fscanf(fp, "%s %s %s %s %d %d %d", nome, cognome, e_mail, password, &data_nascita->tm_mday,
               &data_nascita->tm_mon, &data_nascita->tm_year);

        strcpy(temp1, nome);
        strcpy(temp2, cognome);
        for(int j = 0;j<=strlen(temp1);j++){
            if(temp1[j] == '_'){
                temp1[j] = ' ';
            }
        }
        for(int j = 0;j<=strlen(temp2);j++){
            if(temp2[j] == '_'){
                temp2[j] = ' ';
            }
        }

        tree_client=insertClient(tree_client, temp1, temp2, e_mail, password, data_nascita);
    }

    fclose(fp);
    return tree_client;
}


cliente *addClient(cliente *tree, char *nome, char *cognome, char *email, char *password, struct tm *data_nascita){
    if(tree){
        if(strcmp(tree->e_mail,email)<0){
            tree->right= addClient(tree->right, nome, cognome, email, password, data_nascita);
        }else if(strcmp(tree->e_mail, email)>0){
            tree->left= addClient(tree->left, nome, cognome, email, password, data_nascita);
        }else{
            printf(RED"\nerror"RESET": il cliente e' gia' registrato.\n");
        }
    }else{
        tree= newClient(nome, cognome, email, password, data_nascita);
        addClientFile(nome, cognome, email, password, data_nascita);
    }
    return tree;
}

cliente *insertClient(cliente *tree, char *nome, char *cognome, char *email, char *password, struct tm *data_nascita){
    if(tree){
        if(strcmp(tree->e_mail,email)<0){
            tree->right= insertClient(tree->right, nome, cognome, email, password, data_nascita);
        }else if(strcmp(tree->e_mail, email)>0){
            tree->left= insertClient(tree->left, nome, cognome, email, password, data_nascita);
        }else{
            printf(RED"\nerror"RESET": il cliente e' gia' registrato.\n");
        }
    }else{
        tree= newClient(nome, cognome, email, password, data_nascita);
    }
    return tree;

}


void addClientFile(char *nome, char *cognome, char *email, char *password, struct tm *data_nascita){
    char temp1[MAXNAME], temp2[MAXNAME];
    FILE *fPtr=NULL;

    fflush(stdin);
    fPtr = fopen("clients_list.txt", "a");
    if (fPtr == NULL) {
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }

    strcpy(temp1, nome);
    strcpy(temp2, cognome);
    for(int j = 0;j<=strlen(temp1);j++){
        if(temp1[j] == ' '){
            temp1[j] = '_';
        }
    }
    for(int j = 0;j<=strlen(temp2);j++){
        if(temp2[j] == ' '){
            temp2[j] = '_';
        }
    }

    fprintf(fPtr,"\n%s\t%s\t%s\t%s\t%d %d %d", temp1, temp2, email, password, data_nascita->tm_mday,
            data_nascita->tm_mon, data_nascita->tm_year);
    fclose(fPtr);
}

cliente *newClient(char *nome, char *cognome, char *email, char *password, struct tm *data_nascita){
    cliente *newclient=(cliente *)malloc(sizeof(cliente));
    newclient->data_nascita=malloc(sizeof(struct tm));
    strcpy(newclient->nome, nome);
    strcpy(newclient->cognome, cognome);
    strcpy(newclient->e_mail, email);
    strcpy(newclient->password, password);
    newclient->data_nascita->tm_mday=data_nascita->tm_mday;
    newclient->data_nascita->tm_mon=data_nascita->tm_mon;
    newclient->data_nascita->tm_year=data_nascita->tm_year;
    newclient->punti_sconto=50;
    newclient->right=NULL;
    newclient->left=NULL;
    newclient->listaBiglietti=NULL;

    return newclient;
}

void destroyClients(cliente **tree_client){
    if(*tree_client){
        destroyClients(&(*tree_client)->left);
        destroyClients(&(*tree_client)->right);
        free((*tree_client)->data_nascita);
        destroyTickets(&(*tree_client)->listaBiglietti);
        free(*tree_client);
        *tree_client=NULL;
    }
}

cliente *getClient(cliente *treeClient, char *email){
    if(treeClient){
        if(strcmp(treeClient->e_mail,email)<0){
            return getClient(treeClient->right, email);
        }else if(strcmp(treeClient->e_mail, email)>0){
            return getClient(treeClient->left, email);
        }else{
            return treeClient;
        }
    }else{
        return NULL;
    }
}
//per ogni acquisto, un cliente guadagna il 5% del prezzo pagato in punti
//ogni volta che si fa un acquisto, viene chiesto se spendere i propri punti oppure no
//ogni punto corrisponde allo 0,5% di sconto sul prezzo totale dei biglietti
void buyTicket(cliente **client, volo **fly){

    if(*client){
        if(checkTicket((*client)->listaBiglietti, *fly)==1){

            if((*client)->punti_sconto>0){
                char scelta;

                printf("\nSi desidera utilizzare i propri punti (SALDO ATTUALE: %d pt.)\n"
                       "per l' acquisto del biglietto da "YELLOW"%s"RESET" a "YELLOW"%s"RESET"?  [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ", (*client)->punti_sconto, (*fly)->luogo_partenza, (*fly)->luogo_arrivo);

                takeAnswer(&scelta);
                if(scelta=='s'){
                    float sconto=(float)(*client)->punti_sconto/2;
                    float prezzo_finale=(*fly)->prezzo-(((*fly)->prezzo*sconto)/100);
                    (*fly)->prezzo= prezzo_finale;

                    printf("\nCon lo sconto applicato, il prezzo finale e': %.2f\n", prezzo_finale);
                    (*client)->punti_sconto=0;
                }
            }

            (*client)->listaBiglietti=insertTicket((*client)->listaBiglietti, *fly);
            (*fly)->num_acquirenti+=1;
            printf(GREEN"\n\tPRENOTAZIONE REGISTRATA CON SUCCESSO A %s %s (volo %s-%s)\n"RESET, (*client)->nome, (*client)->cognome, (*fly)->luogo_partenza, (*fly)->luogo_arrivo);
        }else{
            printf(YELLOW"\nwarning"RESET": hai gia' acquistato un biglietto da %s a %s per l'orario indicato\n", (*fly)->luogo_partenza, (*fly)->luogo_arrivo);
            mypause();
        }
    }
}

cliente *signupUser(cliente *tree_client){
    char nome[MAXNAME],
            cognome[MAXNAME],
            e_mail[MAXNAME],
            password[13];
    int uscita=0;

    struct tm *data_nascita=malloc(sizeof(struct tm));
    printf("-----------------------\n");
    printf(YELLOW" EFFETTUA REGISTRAZIONE\n"RESET);
    printf("-----------------------\n\n");
    printf("Nome: ");
    takeSaneString(nome,sizeof(nome));
    printf("Cognome: ");
    takeSaneString(cognome, sizeof(cognome));
    insertEmail(tree_client, e_mail, sizeof(e_mail));
    printf("\nScegli una password: ");
    insertPassword(password, sizeof(password));
    printf("\nData di nascita:\n");
    do{
        printf("\tgiorno: ");
        fflush(stdin);
        if(scanf("%d", &data_nascita->tm_mday)!=1 || data_nascita->tm_mday>31 || data_nascita->tm_mday<=0){
            printf(RED"\nerror"RESET": inserire un valore corretto per il giorno! riprovare\n");
        }else{
            uscita=1;
        }
    }while(uscita!=1);
    uscita=0;
    do{
        printf("\tmese: ");
        fflush(stdin);
        if(scanf("%d", &data_nascita->tm_mon)!=1 || data_nascita->tm_mon>12 || data_nascita->tm_mon<=0){
            printf(RED"\nerror"RESET": inserire un valore corretto per il mese! riprovare\n");
        }else{
            uscita=1;
        }
    }while(uscita!=1);
    uscita=0;
    do{
        printf("\tanno: ");
        fflush(stdin);
        if(scanf("%d", &data_nascita->tm_year)!=1 || data_nascita->tm_year<1880){
            printf(RED"\nerror"RESET": inserire un valore corretto per l'anno! riprovare\n");
        }else{
            uscita=1;
        }
    }while(uscita!=1);
    uscita=0;

    printf("\nRegistrazione in corso dell'utente %s %s (%s) nato il %d/%d/%d... ", nome, cognome, e_mail, data_nascita->tm_mday, data_nascita->tm_mon, data_nascita->tm_year);
    tree_client=addClient(tree_client, nome, cognome, e_mail, password, data_nascita);

    caricamento("", 10);
    printf(GREEN"\n\nUTENTE REGISTRATO CON SUCCESSO!\n"RESET);
    mypause();

    return tree_client;
}

void printClients(cliente *tree_client){
    if(tree_client){
        printf("\n"CYAN"NOME"RESET": %s\n"CYAN"COGNOME"RESET": %s\n"CYAN"E-MAIL"RESET": %s\n"CYAN"DATA DI NASCITA"RESET": %d/%d/%d\n", tree_client->nome, tree_client->cognome, tree_client->e_mail, tree_client->data_nascita->tm_mday, tree_client->data_nascita->tm_mon, tree_client->data_nascita->tm_year);
        printClients(tree_client->left);
        printClients(tree_client->right);
    }
}

void insertEmail(cliente *tree_client, char *e_mail, int size){
    int ch;
    int i = 0, ok=0;

    printf("\nInserisci la tua e-email: ");
    do {
        fflush(stdout);
        while ((ch = getch()) != '\n' && ch != '\r' && i < BUFSIZ - 1) {
            if (ch == '\b' && i > 0) {
                printf("\b \b");
                fflush(stdout);
                i--;
                e_mail[i] = '\0';
            } else {
                putchar(ch);
                e_mail[i++] = (char) ch;
            }
        }
        e_mail[i] = '\0';
        if (getClient(tree_client,e_mail)) {
            for (i = 0; i < strlen(e_mail); i++) {
                printf("\b \b");
                fflush(stdout);
            }
            memset(e_mail,'\0',strlen(e_mail));
            i=0;
            printf("\re-mail inserita gia' utilizzata; inserisci un email valida: ");
        } else
            ok = 1;
    }while(ok==0);

}

void buyTravel(cliente **client, volo **fly){
    if(*client){
        if(*fly){
            buyTicket(client, fly);
            buyTravel(client, &(*fly)->next);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
//TICKET-LIST
void destroyTickets(biglietti **ticket_list){
    if(*ticket_list){
        biglietti *temp= *ticket_list;
        *ticket_list=(*ticket_list)->next;
        free(temp->data_acquisto);
        free(temp);
        temp=NULL;
        destroyTickets(ticket_list);
    }
}

int checkTicket(biglietti *ticket_list, volo *fly){
    if(ticket_list){
        if(equalsDate(ticket_list->fly->data_partenza,fly->data_partenza) && equalsTime(ticket_list->fly->data_partenza,fly->data_partenza) && strcmp(ticket_list->fly->luogo_partenza,fly->luogo_partenza)==0 && strcmp(ticket_list->fly->luogo_arrivo,fly->luogo_arrivo)==0){
            return 0;
        }else{
            return checkTicket(ticket_list->next, fly);
        }
    }
    return 1;
}

biglietti *insertTicket(biglietti *ticketList, volo *fly){
    if(ticketList){
        ticketList->next=insertTicket(ticketList->next,fly);
    }else{
        biglietti *newticket=(biglietti*)malloc(sizeof(biglietti));
        newticket->data_acquisto=(struct tm*)malloc(sizeof(struct tm));
        now(&newticket->data_acquisto->tm_mday, &newticket->data_acquisto->tm_mon, &newticket->data_acquisto->tm_year, &newticket->data_acquisto->tm_hour, &newticket->data_acquisto->tm_min);
        newticket->data_acquisto->tm_sec=0;
        newticket->fly=fly;
        newticket->next=NULL;
        return newticket;
    }
    return ticketList;
}

void printTicketList(biglietti *ticket_list) {
    if(ticket_list){
        printf("biglietto acquistato il %d/%d/%d alle ore %d:%d  da %s per %s che parte il %d/%d/%d %d:%d e arriva il %d/%d/%d %d:%d (pagato %.2f euro)\n",
                ticket_list->data_acquisto->tm_mday, ticket_list->data_acquisto->tm_mon, ticket_list->data_acquisto->tm_year, ticket_list->data_acquisto->tm_hour,
                ticket_list->data_acquisto->tm_min, ticket_list->fly->luogo_partenza, ticket_list->fly->luogo_arrivo, ticket_list->fly->data_partenza->tm_mday,
               ticket_list->fly->data_partenza->tm_mon,ticket_list->fly->data_partenza->tm_year,ticket_list->fly->data_partenza->tm_hour,ticket_list->fly->data_partenza->tm_min,
               ticket_list->fly->data_arrivo->tm_mday,ticket_list->fly->data_arrivo->tm_mon,ticket_list->fly->data_arrivo->tm_year,ticket_list->fly->data_arrivo->tm_hour,
               ticket_list->fly->data_arrivo->tm_min, ticket_list->fly->prezzo);
        printTicketList(ticket_list->next);
    }
}

void printActiveReservations(biglietti *ticket_list){
    if(ticket_list){
        time_t adesso, time_partenza;
        time(&adesso);
        ticket_list->fly->data_partenza->tm_year-=1900;
        ticket_list->fly->data_partenza->tm_mon--;
        time_partenza=mktime(ticket_list->fly->data_partenza);
        ticket_list->fly->data_partenza->tm_year+=1900;
        ticket_list->fly->data_partenza->tm_mon++;

        if(difftime(adesso, time_partenza)<0){

            printf("\nBiglietto acquistato il "YELLOW"%d/%d/%d"RESET" alle ore "YELLOW"%d:%d"RESET"\n\t"CYAN"Partenza"RESET": %s\n\t"CYAN"Arrivo"RESET": %s\n\t"CYAN"Data Partenza"RESET": %d/%d/%d "CYAN"ore "RESET"%d:%d\n\t"CYAN"Data Arrivo"
            RESET": %d/%d/%d "CYAN"ore "RESET"%d:%d\n\t"CYAN"Importo"RESET": %.2f euro\n",
                   ticket_list->data_acquisto->tm_mday, ticket_list->data_acquisto->tm_mon, ticket_list->data_acquisto->tm_year, ticket_list->data_acquisto->tm_hour,
                   ticket_list->data_acquisto->tm_min, ticket_list->fly->luogo_partenza, ticket_list->fly->luogo_arrivo, ticket_list->fly->data_partenza->tm_mday,
                   ticket_list->fly->data_partenza->tm_mon,ticket_list->fly->data_partenza->tm_year,ticket_list->fly->data_partenza->tm_hour,ticket_list->fly->data_partenza->tm_min,
                   ticket_list->fly->data_arrivo->tm_mday,ticket_list->fly->data_arrivo->tm_mon,ticket_list->fly->data_arrivo->tm_year,ticket_list->fly->data_arrivo->tm_hour,
                   ticket_list->fly->data_arrivo->tm_min, ticket_list->fly->prezzo);
        }
        printActiveReservations(ticket_list->next);
    }
}