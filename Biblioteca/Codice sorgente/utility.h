#ifndef BIBLIOTECA_UTILITY_H
#define BIBLIOTECA_UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Colori utilizzati per le stampe
#define GREEN "\x1b[1;32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[36m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define GRAY "\x1b[90m"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//prende da stdin un array di caratteri e sostituisce \n del tasto invio con \0
void getStr(char *str, int len);

//fotografa l' orario di sistema e lo salva in variabili intere
void now(int *giorno, int* mese, int *anno, int *ora, int* minuti);

//funzione volta a rendere più piacevole l' interfaccia utente
void mypause(void);

//permette di ritornare un inserimento da tastiera limitato ai caratteri 'S' e 'N'
void takeAnswer(char *answer);

//evita l' immissione di nomi, cognomi e titoli vuoti
void takeSaneString(char *string, int size);

//obbliga l' utente a immettere una matricola di 10 caratteri
void takeSaneIDStudent(char *matricola, int size);

#endif //BIBLIOTECA_UTILITY_H
