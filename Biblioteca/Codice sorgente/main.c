#include <unistd.h>
#include "bookbst.h"
#include "studentbst.h"
#include "queuerequest.h"

//stampe a schermo per la proiezione del menu
void menu(void);

int main(void) {
    book *library=NULL, *librorichiesto=NULL;
    student *students=NULL, *richiedente=NULL;
    request *requests=NULL;
    int sceltaMenu=0, check, err;
    char nome[50], cognome[50], matricola[10], titolo[100];
    char scelta='\0';
    library=initializeLibrary();
    system("cls");
    do{
        sceltaMenu=0;
        menu();
        fflush(stdin);
        scanf("%d", &sceltaMenu);
        system("cls");
        switch(sceltaMenu){
            case -1:{
                printf("\nUscita in corso...\n\n");
                destroyLibrary(&library);
                destroyStudents(&students);
                destroyRequests(&requests);
                sleep(1);
                break;
            }
            case 1:{
                check= 0;
                printf("Inserire matricola dello studente richiedente il prestito"GRAY" [-1 per tornare al menu]"RESET": ");
                takeSaneIDStudent(matricola, sizeof(matricola));
                if(strcmp(matricola,"-1")!=0) {
                    richiedente= getStudent(students, matricola);
                    if (!richiedente){
                        printf(YELLOW"\tattenzione"RESET": lo studente non e' registrato alla biblioteca e per noleggiare un libro questo e' necessario.\n\tSi desidera registrarlo ?"GRAY"[S/N]"RESET": ");
                        takeAnswer(&scelta);
                        if (scelta == 'S') {
                            //lo studente ha scelto di registrarsi alla biblioteca
                            printf("\tInserire nome studente: ");
                            takeSaneString(nome, sizeof(nome));
                            printf("\tInserire cognome studente: ");
                            takeSaneString(cognome, sizeof(cognome));
                            students = addStudent(students, nome, cognome, matricola);
                            richiedente = getStudent(students, matricola);
                            printf(GREEN"\tStudente registrato con successo !\n"RESET);
                        } else {
                            //lo studente ha scelto di non registrarsi
                            printf(RED"\tattenzione"RESET": i servizi della biblioteca sono garantiti soltanto agli studenti registrati.\n");
                        }
                    }
                    if (richiedente) {
                        //fintanto che check è -1
                        do {
                            do {
                                printf("Inserire il nome del libro da noleggiare"GRAY" [+info, per consultare il catalogo]"RESET": ");
                                takeSaneString(titolo, sizeof(titolo));
                                if (strcmp(titolo, "+info") == 0) {
                                    system("cls");
                                    inOrderStamp(library);
                                }
                            } while (strcmp(titolo, "+info") == 0);
                            librorichiesto = getBook(library, titolo);
                            if (librorichiesto) {
                                //se il titolo del libro inserito appartiene a un libro della biblioteca
                                if (librorichiesto->availability > 0) {
                                    if (checkLoans(richiedente->borrowedbooks, librorichiesto->title) == 1){
                                        //il libro richeisto non è già stato noleggiato dallo studente
                                        rentBook(&richiedente, librorichiesto);
                                    }else{
                                        printf(YELLOW"attenzione"RESET": lo studente gia' ha ricevuto in prestito una copia del libro\n");
                                    }
                                }else{
                                    printf(YELLOW"\tattenzione"RESET": attualmente non sono disponibili copie del libro %s in biblioteca.\n",
                                           librorichiesto->title);
                                    printf("\tLo studente desidera prenotare il libro ?"GRAY"[S/N]"RESET": ");
                                    takeAnswer(&scelta);
                                    if (scelta == 'S' && checkLoans(richiedente->borrowedbooks, librorichiesto->title) == 1) {
                                        check = checkRequest(requests, richiedente->matricola, librorichiesto->title);
                                        if (check == 1) {
                                            //questa richiesta non è già stata fatta dallo studente
                                            requests = insertRequest(requests, library, richiedente, librorichiesto);
                                        }
                                    }else{
                                        if(checkLoans(richiedente->borrowedbooks, librorichiesto->title) == 0){
                                            //lo studente ha già il libro
                                            printf(YELLOW"\tattenzione"RESET": lo studente ha gia' ricevuto in prestito il libro (non e' possibile procedere con la prenotazione)\n");
                                        }
                                    }
                                }
                                check = 0;
                            }else{
                                printf(RED"\terrore"RESET": il libro richiesto non e' nella collezione della biblioteca.\n");
                                printf("\tSi vuole riprovare a scrivere il titolo del libro ?"GRAY"[S/N]"RESET":");
                                takeAnswer(&scelta);
                                if (scelta == 'S') {
                                    check = -1;
                                } else {
                                    system("cls");
                                    printf(RED"\terrore"RESET": senza un titolo non e' possibile continuare con l'operazione.\n");
                                    check = 0;
                                }
                            }
                        } while (check == -1);
                    }
                    mypause();
                }
                system("cls");
                break;
            }
            case 2:{
                check=0;
                scelta='N';
                do {
                    printf("\n\nInserire la matricola dello studente"GRAY" [-1 per tornare al menu]"RESET": ");
                    takeSaneIDStudent(matricola, sizeof(matricola));
                    if (strcmp(matricola, "-1") != 0) {
                        richiedente = getStudent(students, matricola);
                        if (!richiedente) {
                            printf(YELLOW"\tattenzione"RESET": lo studente non e' registrato e quindi e' impossibile che stia restituendo un libro della bibioteca.\n\tSi desidera riscrivere la matricola ?"GRAY"[S/N]"RESET": ");
                            takeAnswer(&scelta);
                        } else {
                            //fintanto che check è -1
                            do {
                                do {
                                    printf("Inserire il nome del libro da restituire"GRAY"[+info, per consultare il catalogo]"RESET": ");
                                    takeSaneString(titolo, sizeof(titolo));
                                    if (strcmp(titolo, "+info") == 0) {
                                        system("cls");
                                        inOrderStamp(library);
                                    }
                                } while (strcmp(titolo, "+info") == 0);
                                librorichiesto = getBook(library, titolo);
                                if (librorichiesto) {
                                    if (checkLoans(richiedente->borrowedbooks, librorichiesto->title) == 0) {
                                        //il libro è stato dato in prestito allo studente richiedente
                                        returnBook(&richiedente, librorichiesto, &err);
                                        if (err == 0) {
                                            printf(GREEN"\tLibro consegnato con successo"RESET": adesso disponibili %d copie di %s\n",
                                                   librorichiesto->availability, librorichiesto->title);
                                            printf("\tcontrollo se il libro e' stato prenotato...\n");
                                            sleep(2);
                                            requests = checkDeleteRequest(requests, librorichiesto);
                                        }
                                    } else {
                                        printf(YELLOW"attenzione"RESET": non risulta che il libro %s sia stato noleggiato a %s\n",
                                               librorichiesto->title, richiedente->matricola);
                                    }
                                    if (err == 0) {
                                        check = 0;
                                    } else {
                                        printf("\t"RED"errore"RESET": Si vuole riprovare a scrivere il titolo del libro ?"GRAY"[S/N]"RESET":");
                                        takeAnswer(&scelta);
                                        if (scelta == 'S') {
                                            check = -1;
                                        } else {
                                            system("cls");
                                            printf(RED"\terrore"RESET": senza un titolo non e' possibile continuare con l'operazione.\n");
                                            check = 0;
                                        }
                                    }
                                } else {
                                    printf(RED"\terrore"RESET": il libro richiesto non e' nella collezione della biblioteca.\n");
                                    printf("\tSi vuole riprovare a scrivere il titolo del libro ?"GRAY"[S/N]"RESET":");
                                    takeAnswer(&scelta);
                                    if (scelta == 'S') {
                                        check = -1;
                                        system("cls");
                                    } else {
                                        printf(RED"\terrore"RESET": senza un titolo non e' possibile continuare con l'operazione.\n");
                                        check = 0;
                                    }
                                }
                            } while (check == -1);
                        }
                        mypause();
                    }
                }while(!richiedente && scelta=='S' && strcmp(matricola,"-1")!=0);
                system("cls");
                break;
            }
            case 3: {
                printf(CYAN"\t\t\tStudenti iscritti\n"RESET);
                if(students) {
                    studentStamp(students);
                }else{
                    printf(GRAY"\t\t\t   (nessuno)\n"RESET);
                }
                printf("\n");
                mypause();
                system("cls");
                break;
            }
            case 4: {
                do {
                    scelta='N';
                    printf("\n\nInserire la matricola dello studente"GRAY" [-1 per tornare al menu]"RESET": ");
                    takeSaneIDStudent(matricola, sizeof(matricola));
                    if(strcmp(matricola,"-1")!=0) {
                        richiedente = getStudent(students, matricola);
                        if (richiedente) {
                            printf(CYAN"\n----------------\tLibri prestati a %s\t----------------\n"RESET,richiedente->matricola);
                            if (richiedente->borrowedbooks) {
                                stampaLibriPosseduti(richiedente);
                                printf("\n");
                            } else {
                                printf(GRAY"\t\t\t\t\t(nessuno)\n\n"RESET);
                            }
                        } else {
                            printf("\tLo studente non e' registrato alla biblioteca e per questo non ha noleggiato alcun libro\n");
                        }
                        printf("\tSi desidera avere informazioni riguardo i prestiti fatti ad un altro studente ?" GRAY"[S/N]"RESET":");
                        takeAnswer(&scelta);
                    }
                } while (scelta != 'N');
                mypause();
                system("cls");
                break;
            }
            case 5: {
                do {
                    scelta='N';
                    printf("Inserire il titolo del libro"GRAY" [-1 per tornare al menu]"RESET": ");
                    takeSaneString(titolo, sizeof(titolo));
                    if(strcmp(titolo,"-1")!=0) {
                        librorichiesto = getBook(library, titolo);
                        if (librorichiesto) {
                            printf("\t\t~>%d copie disponibili\n", librorichiesto->availability);
                        } else {
                            printf("\tIl libro richiesto non e' nella collezione della biblioteca\n");
                        }
                        printf("Si desidera avere informazioni riguardo le copie di un altro libro ?"GRAY"[S/N]"RESET":");
                        takeAnswer(&scelta);
                    }
                } while (scelta != 'N');
                mypause();
                system("cls");
                break;
            }
            case 6: {
                printf(YELLOW"~~~~~~~~~~~~~~~~\tCollezione della biblioteca \t~~~~~~~~~~~~~~~~~~~~~~~~~\n"RESET);
                if(library) {
                    inOrderStamp(library);
                }else{
                    printf(GRAY"\t\t\t\t   (vuoto)\n"RESET);
                }
                break;
            }
            case 7: {
                printf(CYAN"----------------\tPrenotazioni dalla meno recente \t----------------\n"RESET);
                if (requests) {
                    queueStamp(requests);
                } else {
                    printf(GRAY"\t\t\t\t   (vuoto)\n"RESET);
                }
                mypause();
                system("cls");
                break;
            }
            case 8: {
                do {
                    check=0;
                    err=0;
                    scelta='N';
                    printf("\n\nInserire la matricola dello studente che si vuole sollecitare"GRAY" [-1 per tornare al menu]"RESET": ");
                    takeSaneIDStudent(matricola, sizeof(matricola));
                    if(strcmp(matricola,"-1")!=0) {
                        richiedente = getStudent(students, matricola);
                        if (richiedente){
                            do {
                                do {
                                    printf("Inserire il nome del libro che si desidera far consegnare "GRAY"[+info, per consultare il catalogo]"RESET": ");
                                    takeSaneString(titolo, sizeof(titolo));
                                    if (strcmp(titolo, "+info") == 0) {
                                        system("cls");
                                        inOrderStamp(library);
                                    }
                                } while (strcmp(titolo, "+info") == 0);
                                librorichiesto = getBook(library, titolo);
                                if (librorichiesto) {
                                    //il libro rischiesto esiste quindi controllo se è tra i libri noleggiati allo studente
                                    forcedReturnBook(&richiedente,librorichiesto,&err);
                                    if (err == 0) {
                                        printf(GREEN"\tLibro consegnato con successo"RESET": adesso disponibili %d copie di %s\n",librorichiesto->availability, librorichiesto->title);
                                        //controllo che il libro sia tra quelli prenotati e nel caso lo consegno al primo studente con checkDeleteRequest in queuerequest.h
                                        printf("\tcontrollo se il libro e' stato prenotato...\n");
                                        sleep(2);
                                        requests = checkDeleteRequest(requests, librorichiesto);
                                    }
                                    check = 0; //lascialo
                                } else {
                                    printf(RED"\tIl libro digitato non e' nella collezione della biblioteca.\n"RESET);
                                    printf("\tSi vuole riprovare a scrivere il titolo del libro di cui si desidera la consegna ?"GRAY"[S/N]"RESET":");
                                    takeAnswer(&scelta);
                                    if (scelta == 'S') {
                                        check = -1;
                                    } else {
                                        system("cls");
                                        printf(RED"\tSenza un titolo non e' possibile continuare con l'operazione.\n"RESET);
                                        check = 0;
                                    }
                                }
                            } while (check == -1);

                        } else {
                            printf("\tLo studente non e' registrato alla biblioteca e per questo non puo' aver noleggiato alcun libro.\n");
                        }
                        printf("\tSi desidera sollecitare un altro studente ?" GRAY"[S/N]"RESET":");
                        takeAnswer(&scelta);
                    }
                } while (scelta != 'N');
                mypause();
                system("cls");
                break;
            }
            default:{
                system("cls");
                printf(RED"--> Immettere uno tra i numeri indicati tra parentesi\n"RESET);
                mypause();
                system("cls");
            }
        }
        fflush(stdin);
    }while(sceltaMenu!=-1);

    return 0;
}

void menu(){
    printf(BLUE"    * * * * * * * * * * * * * * * * * "RED);
    printf("MENU ");
    printf(BLUE"* * * * * * * * * * * * * * * * * *\n");
    printf("    *   "CYAN"\t\t[1]"RESET"  Nuovo prestito");
    printf(BLUE"\t\t\t\t     *\n");
    printf("    *   "CYAN"\t\t[2]"RESET"  Consegna libro dato in prestito");
    printf(BLUE"\t\t     *\n");
    printf("    *   "CYAN"\t\t[3]"RESET"  Studenti registrati in biblioteca");
    printf(BLUE"\t\t     *\n");
    printf("    *   "CYAN"\t\t[4]"RESET"  Libri noleggiati ad uno studente");
    printf(BLUE"\t\t     *\n");
    printf("    *   "CYAN"\t\t[5]"RESET"  Copie disponibili di un certo libro");
    printf(BLUE"\t     *\n");
    printf("    *   "CYAN"\t\t[6]"RESET"  Collezione della biblioteca");
    printf(BLUE"\t\t     *\n");
    printf("    *   "CYAN"\t\t[7]"RESET"  Prenotazioni degli studenti");
    printf(BLUE"\t\t     *\n");
    printf("    *   "CYAN"\t\t[8]"RESET"  Sollecita uno studente alla consegna");
    printf(BLUE"\t     *\n");
    printf("    *   "CYAN"\t\t[-1]"RESET" Esci");
    printf(BLUE"\t\t\t\t             *"CYAN"\n\t\t\t->"RESET);
}