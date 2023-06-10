#ifndef PROGETTOII_CLIENTBST_H
#define PROGETTOII_CLIENTBST_H

#include "utility.h"
#include "flightsLists.h"

typedef struct biglietti{
    volo *fly;
    struct tm *data_acquisto;
    struct biglietti *next;
}biglietti;

typedef struct cliente{
    char nome[MAXNAME],
            cognome[MAXNAME],
            e_mail[MAXEMAIL],
            password[13];
    struct tm *data_nascita;
    biglietti *listaBiglietti;
    int punti_sconto;
    struct cliente *right,
            *left;
}cliente;

//inizializza l'albero dei clienti
cliente *initClients(void);

//aggiunge un nuovo cliente all'albero
cliente *addClient(cliente *tree, char *nome, char *cognome, char *email, char *password, struct tm *data_nascita);

//inserisce un nuovo cliente all'albero senza registrare l'operazione nel file "clients_list.txt"
cliente *insertClient(cliente *tree, char *nome, char *cognome, char *email, char *password, struct tm *data_nascita);

//alloca in memoria il nuovo cliente
cliente *newClient(char *nome, char *cognome, char *email, char *password, struct tm *data_nascita);

//dealloca ogni cliente dalla memoria
void destroyClients(cliente **tree_client);

//ritorna il puntatore a nodo del cliente con e-mail in ingresso
cliente *getClient(cliente *treeClient, char *email);

//acquisto di un biglietto da parte del cliente --- aggiunta del ticket alla listaBiglietti
void buyTicket(cliente **client, volo **fly);

void addClientFile(char *nome, char *cognome, char *email, char *password, struct tm *data_nascita);

//registra un nuovo cliente
cliente *signupUser(cliente *tree_client);

//stampa a schermo i clienti registrati
void printClients(cliente *tree_client);

void insertEmail(cliente *tree_client, char *e_mail, int size);

void buyTravel(cliente **client, volo **fly);
////////////////////////////////////////////////////////////////////////////////////////////////////
//TICKET-LIST

//dealloca dalla memoria ogni biglietto
void destroyTickets(biglietti **ticketList);

//ritorna 1 se non Ã¨ giÃ  presente un biglietto con i campi in ingresso nella lista
int checkTicket(biglietti *ticket_list, volo *fly);

//inserisce un nuovo ticket tra quelli della lista (biglietti posseduti da un cliente)
biglietti *insertTicket(biglietti *ticketList, volo *fly);

//stampa a schermo dei biglietti acquistati da un cliente
void printTicketList(biglietti *ticket_list);

//stampa a schermo dei biglietti acquistati da un cliente ancora validi
void printActiveReservations(biglietti *ticket_list);
#endif //PROGETTOII_CLIENTBST_H