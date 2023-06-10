#ifndef PROGETTOII_UTILITY_H
#define PROGETTOII_UTILITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>
#include <limits.h>
#include <float.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Costanti utilizzate
#define MAXNAME 40
#define MAXEMAIL 100
#define PWSIZE 13
#define IDSIZE 10
#define BUFFER_SIZE 50
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//macro utilizzate
#define FLOAT_TO_INT(x) ((x)>=0?(int)((x)+0.5):(int)((x)-0.5)) //arrotondamento
//Colori utilizzati per le stampe
#define GREEN "\x1b[1;32m"
#define RED "\x1b[31m"
#define YELLOW "\x1b[1;33m"
#define CYAN "\x1b[36m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define GRAY "\x1b[90m"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//prende da stdin un array di caratteri e sostituisce \n del tasto invio con \0
void getStr(char *str, int len);

//prende una stringa e modifica l'ultimo carattere
void modifyString(char *str1, char *str2);

//fotografa l' orario di sistema e lo salva in variabili intere
void now(int *giorno, int* mese, int *anno, int *ora, int* minuti);

//funzione volta a rendere più piacevole l' interfaccia utente
void mypause(void);

//permette di ritornare un inserimento da tastiera limitato ai caratteri 's' e 'n'
void takeAnswer(char *answer);

//permette di ritornare un inserimento da tastiera limitato ai caratteri 's' e 'n' e '0'
void takeAnswerMenu(char *answer);

//evita l' immissione di nomi, cognomi e titoli vuoti
void takeSaneString(char *string, int size);

int equalsDate(struct tm *data1, struct tm *data2);

int equalsTime(struct tm *orario1, struct tm *orario2);

void insertPassword(char *pword, int size);

//restituisce uno se value è nell'array
int checkOccurrence(int *array, int size, int value);

void twirlybar(void);

void caricamento(char *message, int reps);

void clearScreen(void);

#endif //PROGETTOII_UTILITY_H