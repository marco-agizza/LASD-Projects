#include "clientBST.h"
#include "operatorBST.h"
#include "routeGraph.h"
#include "pathList.h"

int login(cliente *tree_cliente, operatore *tree_operator, void **logged_user);
void menuClient(char *nome, char *cognome, char *e_mail, int punti);
void menuGuest();
void menuOperator(char *id_operator);

int main() {
    system("cls");
    int log_in_value;
    void *logged_user;
    char message[100];
    cliente *tree_client=NULL;
    operatore *tree_operator=NULL;
    tratta *cities_graph=initCities();
    tree_operator= initializeOperators();
    tree_client=initClients();
    initFlights(cities_graph);

    do{
        cliente *logged_client = NULL;
        operatore *logged_operator = NULL;
        log_in_value = login(tree_client, tree_operator, &logged_user);

        switch(log_in_value){

            case 1: {
                //si e' loggato un cliente
                int scelta_menu = 0;
                int uscita = 1;
                logged_client = (cliente *) logged_user;
                logged_client->listaBiglietti=updateListaBiglietti(cities_graph, &logged_client, logged_client->listaBiglietti);

                system("cls");
                do {
                    uscita =1;
                    menuClient(logged_client->nome, logged_client->cognome, logged_client->e_mail, logged_client->punti_sconto);

                    if (scanf("%d", &scelta_menu) != 1) {
                        scelta_menu = -1;
                    }

                    switch (scelta_menu) {
                        case 1: {
                            //il cliente ha scelto di cercare la disponibilita' di un volo oppure
                            //ha scelto di acquistare un biglietto
                            int choice = 0;
                            char src[MAXNAME],
                                    dest[MAXNAME];
                            int *reachable_nodes = NULL;
                            struct tm *fascia_oraria = NULL;
                            int most_pop = 0, cheapest = 0;
                            char most_pop_city[MAXNAME],
                                    cheapest_city[MAXNAME];
                            paths *possible_paths = NULL;
                            flight_path *possible_travels = NULL;

                            fascia_oraria = malloc(sizeof(struct tm));
                            fascia_oraria->tm_isdst = -1;
                            fascia_oraria->tm_sec = 0;
                            fascia_oraria->tm_min = 0;

                            do {
                                system("cls");
                                printf("------------\n");
                                printf(YELLOW" RICERCA VOLO\n"RESET);
                                printf("------------\n");
                                printf("\nInserire l'aeroporto di partenza: ");
                                takeSaneString(src, MAXNAME);
                                if (findArrayPosition(cities_graph, src) == -1) {
                                    printf(RED"error"RESET": la citta' inserita non e' disponibile\n");
                                    mypause();
                                }
                            } while (findArrayPosition(cities_graph, src) == -1);

                            reachable_nodes = reachableNodes(cities_graph, src);
                            most_pop = findMostPopularDestination(cities_graph, reachable_nodes);
                            cheapest = findCheapestDestination(cities_graph, reachable_nodes, src);
                            strcpy(most_pop_city, findNameVertex(cities_graph, most_pop));
                            strcpy(cheapest_city, findNameVertex(cities_graph, cheapest));

                            do {
                                system("cls");
                                printf("-------------\n");
                                printf(YELLOW" RICERCA VOLO\n"RESET);
                                printf("-------------\n");
                                printf("\nHai scelto di partire da %s", src);
                                printf(YELLOW"\n\nCaro cliente, ti suggeriamo %s che e' la meta piu' economica partendo da %s"RESET,
                                       cheapest_city, src);
                                printf(YELLOW"\nPotrebbe interessarti la meta piu' gettonata che e' %s, approfittane!\n"RESET,
                                       most_pop_city);
                                printf("\nInserire l'aeroporto di arrivo: ");
                                takeSaneString(dest, MAXNAME);
                                if (findArrayPosition(cities_graph, dest) == -1) {
                                    printf(RED"error"RESET": la citta' inserita non e' disponibile, riprova\n");
                                    mypause();
                                }
                            } while (findArrayPosition(cities_graph, dest) == -1);

                            printf("\nDati per la partenza:\n");

                            do {
                                printf("\n\tgiorno: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_mday) != 1 || fascia_oraria->tm_mday > 31 ||
                                    fascia_oraria->tm_mday <= 0) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per il giorno!");
                                } else {
                                    uscita = 0;
                                }
                            } while (uscita != 0);
                            uscita = 1;
                            do {
                                printf("\n\tmese: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_mon) != 1 || fascia_oraria->tm_mon > 12 ||
                                    fascia_oraria->tm_mon <= 0) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per il mese! riprova");
                                } else {
                                    uscita = 0;
                                }
                            } while (uscita != 0);
                            uscita = 1;
                            do {
                                printf("\n\tanno: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_year) != 1 || fascia_oraria->tm_year < 2020) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per l'anno! riprova");
                                } else {
                                    uscita = 0;
                                }
                            } while (uscita != 0);
                            uscita = 1;
                            do {
                                printf("\n\tfascia oraria: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_hour) != 1 || fascia_oraria->tm_hour < 0 ||
                                    fascia_oraria->tm_hour > 23) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per la fascia oraria!");
                                } else {
                                    uscita = 0;
                                }
                            } while (uscita != 0);
                            uscita = 1;

                            possible_paths = possiblePaths(cities_graph, src, dest);
                            possible_travels = makeFlightSolutions(cities_graph, possible_paths, fascia_oraria);
                            system("cls");
                            printf("------------\n");
                            printf(YELLOW" RICERCA VOLO\n"RESET);
                            printf("------------\n");

                            if(possible_travels){
                                printf(GREEN"\tVOLI DISPONIBILI PER LA TRATTA: %s - %s\n"RESET, src, dest);
                                printf("--------------------------------------------------------------------\n");
                                printf(YELLOW"COD.\tDATA PARTENZA\tORARIO\t\t\tDATA ARRIVO\tORARIO\t\t\tPREZZO(euro)\n\n"RESET);
                                printFlightsPath(possible_travels, 1);
                                printf("\n\nIl "GREEN"viaggio piu' breve"RESET" tra %s e %s e' il seguente:\n\t\t", src, dest);
                                path *shortest_path= shortestDistancePath(cities_graph, src, dest);
                                printPathCities(cities_graph, shortest_path);
                                printf("\n\nIl "GREEN"viaggio piu' economico"RESET" tra %s e %s e' il seguente:\n\t\t", src, dest);
                                path *cheapest_travel= lowestPricePath(cities_graph,src,dest);
                                printPathCities(cities_graph, cheapest_travel);
                                printf("\nSi desidera procedere con l'acquisto di un biglietto o si vuole tornare al menu' principale?");
                                printf("\n[ "GREEN"1=Procedi con l'acquisto"RESET" | "YELLOW"0=Torna al menu' principale"RESET"]\n");

                                do {
                                    printf("\nINSERISCI QUI LA TUA SCELTA ---> ");
                                    fflush(stdin);
                                    if (scanf("%d", &choice) != 1) {
                                        printf(RED"\nerror"RESET": inserire 1 o 0 a seconda della scelta che si vuole effettuare!");
                                    } else {
                                        if (choice < 0 || choice > 1) {
                                            printf(RED"\nerror"RESET": inserire 1 o 0 a seconda della scelta che si vuole effettuare!");
                                        } else {
                                            uscita = 0;
                                        }
                                    }
                                } while (uscita != 0);
                                uscita=1;


                                if (choice == 1) {
                                    //il cliente ha deciso di proseguire con l'acquisto del biglietto
                                    int numero_soluzione;

                                    do {
                                        printf("\nInserire il "GREEN"codice"RESET" del viaggio che si intende acquistare: ");
                                        fflush(stdin);
                                        if (scanf("%d", &numero_soluzione) != 1) {
                                            printf(RED"\nerror"RESET": inserire uno tra i codici viaggio evidenziati\n");
                                        } else {
                                            if (numero_soluzione > getNumTravel(possible_travels) || numero_soluzione <= 0) {
                                                printf(RED"\nerror"RESET": inserire uno tra i codici viaggio evidenziati\n");
                                            }
                                        }
                                    } while (numero_soluzione > getNumTravel(possible_travels) || numero_soluzione <= 0);

                                    volo *lista_da_acquistare = NULL;
                                    lista_da_acquistare = getTravel(possible_travels, numero_soluzione);
                                    buyTravel(&logged_client, &lista_da_acquistare);
                                    mypause();
                                    printf(GREEN"\nACQUISTO EFFETTUATO CON SUCCESSO!"RESET);
                                }
                            }else
                                printf("\n\t\tSpiacenti! Non sono disponibili voli per la combinazione richiesta\n"RESET);


                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 13);
                            break;
                        }

                        case 2: {
                            //il cliente ha scelto di visualizzare le destinazioni disponibili
                            system("cls");
                            printf("-----------------------\n");
                            printf(YELLOW" VISUALIZZA DESTINAZIONI\n"RESET);
                            printf("-----------------------\n");

                            printf("\nLe destinazioni disponibili sono:\n\n");
                            printCities(cities_graph);
                            printf("\n");
                            mypause();
                            break;
                        }

                        case 3: {
                            //il cliente ha scelto di visualizzare le sue prenotazioni attive
                            system("cls");
                            printf("------------------\n");
                            printf(YELLOW" PRENOTAZIONI ATTIVE\n"RESET);
                            printf("------------------\n");

                            printf("\nLe tue prenotazioni attive sono:\n");

                            if(logged_client->listaBiglietti)
                                printActiveReservations(logged_client->listaBiglietti);
                            else
                                printf("Nessun biglietto acquistato\n");
                            printf("\n");
                            mypause();
                            break;
                        }

                        case 0: {
                            //il cliente ha deciso di effettuare il logout
                            strcpy(message, "\nGrazie per aver utilizzato il programma!\nLogout in corso... ");
                            caricamento(message, 10);
                            uscita = 0;
                            break;
                        }

                        default: {
                            //il cliente ha cliccato un tasto non valido
                            printf(RED"error"RESET": e' stata inserita una scelta non valida...\n");
                            mypause();
                            break;
                        }
                    }
                } while (uscita != 0);
                break;
            }

            case 2 :{
                //si e' loggato un operatore
                fflush(stdout);
                system("cls");
                int scelta_menu = 0;
                int uscita = 0;
                int err = 0;

                logged_operator = (operatore *) logged_user;
                do {

                    menuOperator(logged_operator->id_operator);
                    err=0;

                    if (scanf("%d", &scelta_menu) != 1) {
                        scelta_menu = -1;
                    }

                    switch (scelta_menu) {
                        case 1: {
                            //l'operatore ha scelto di aggiungere una nuova tratta
                            do{
                                err=0;
                                system("cls");
                                printf("------------------\n");
                                printf(YELLOW" INSERIMENTO TRATTA\n"RESET);
                                printf("------------------\n");

                                char src[MAXNAME],
                                        dest[MAXNAME];
                                char scelta;
                                int distance=0;

                                printf("\nInserire i dati della tratta che si vuole aggiungere:");
                                printf("\n\n\tCitta' di partenza: ");
                                takeSaneString(src, MAXNAME);
                                printf("\n\tCitta' di arrivo: ");
                                takeSaneString(dest, MAXNAME);
                                printf("\n\tDistanza (in km): ");
                                while(scanf("%d", &distance)!=1){
                                    printf(RED"error"RESET": inserire un valore valido per la distanza.");
                                    printf("\n\tDistanza (in km): ");
                                    fflush(stdin);
                                }
                                addRoute(cities_graph, src, dest, distance,&err);
                                if(err==0){
                                    printf(GREEN"\nLa tratta da %s a %s e' stata inserita correttamente nel sistema!\n"RESET, src, dest);
                                }else{
                                    printf("\nSi vuole riprovare ad inserire una nuova tratta? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if(scelta =='n')
                                        err=0;
                                }
                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);

                            break;
                        }

                        case 2: {
                            do{
                                err=0;
                                //l'operatore ha scelto di rimuovere una tratta esistente
                                system("cls");
                                printf("----------------\n");
                                printf(YELLOW" RIMOZIONE TRATTA\n"RESET);
                                printf("----------------\n");
                                char src[MAXNAME],
                                        dest[MAXNAME];
                                char scelta;

                                printf("\nInserire i dati della tratta che si vuole rimuovere:");
                                printf("\n\n\tCitta' di partenza: ");
                                takeSaneString(src, MAXNAME);
                                printf("\n\tCitta' di arrivo: ");
                                takeSaneString(dest, MAXNAME);
                                removeRoute(cities_graph, src, dest,&err);
                                if(err==0){
                                    printf(GREEN"\nLa tratta da %s a %s e' stata rimossa correttamente nel sistema!\n"RESET, src, dest);
                                }else{
                                    printf("\nSi vuole riprovare a rimuovere una tratta? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if(scelta =='n')
                                        err=0;
                                }
                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);

                            break;
                        }

                        case 3: {
                            //l'operatore ha  scelto di aggiungere un nuovo volo
                            do{
                                err=0;
                                char src[MAXNAME],
                                        dest[MAXNAME],
                                        scelta;
                                float prezzo;
                                struct tm *data_partenza=malloc(sizeof(struct tm));
                                struct tm *data_arrivo=malloc(sizeof(struct tm));
                                data_partenza->tm_isdst=-1;
                                data_arrivo->tm_isdst=-1;
                                data_partenza->tm_sec=0;
                                data_arrivo->tm_sec=0;
                                time_t time_data_partenza, time_data_arrivo;
                                do {
                                    system("cls");
                                    printf("----------------\n");
                                    printf(YELLOW" INSERIMENTO VOLO\n"RESET);
                                    printf("----------------\n");
                                    printf("\nInserire i dati del volo che si vuole inserire:");
                                    printf("\n\n\tCitta' di partenza: ");
                                    takeSaneString(src, MAXNAME);
                                    printf("\n\tCitta' di arrivo: ");
                                    takeSaneString(dest, MAXNAME);
                                    printf("\n\tData partenza:\n");
                                    printf("\t\tgiorno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_mday) != 1 || data_partenza->tm_mday <= 0 ||
                                            data_partenza->tm_mday > 31) {
                                            printf(RED"error"RESET": inserire un valore valido per il giorno.\n");
                                            printf("\t\tgiorno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tmese: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_mon) != 1 || data_partenza->tm_mon < 1 ||
                                            data_partenza->tm_mon > 12) {
                                            printf(RED"error"RESET": inserire un valore valido per il mese.\n");
                                            printf("\t\tmese: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tanno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_year) != 1 ||
                                            data_partenza->tm_year < 2020) {
                                            printf(RED"error"RESET": inserire un valore valido per l'anno.\n");
                                            printf("\t\tanno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tora: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_hour) != 1 || data_partenza->tm_hour < 0 ||
                                            data_partenza->tm_hour > 23) {
                                            printf(RED"error"RESET": inserire un valore valido per l'ora.\n");
                                            printf("\t\tora: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tminuti: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_min) != 1 || data_partenza->tm_min < 0 ||
                                            data_partenza->tm_min > 59) {
                                            printf(RED"error"RESET": inserire un valore valido per i mini.\n");
                                            printf("\t\tminuti: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;

                                    printf("\n\tData arrivo:\n");
                                    printf("\t\tgiorno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_mday) != 1 || data_arrivo->tm_mday <= 0 ||
                                            data_arrivo->tm_mday > 31) {
                                            printf(RED"error"RESET": inserire un valore valido per il giorno.\n");
                                            printf("\t\tgiorno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tmese: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_mon) != 1 || data_arrivo->tm_mon < 1 ||
                                            data_arrivo->tm_mon > 12) {
                                            printf(RED"error"RESET": inserire un valore valido per il mese.\n");
                                            printf("\t\tmese: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tanno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_year) != 1 || data_arrivo->tm_year < 2020) {
                                            printf(RED"error"RESET": inserire un valore valido per l'anno.\n");
                                            printf("\t\tanno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tora: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_hour) != 1 || data_arrivo->tm_hour < 0 ||
                                            data_arrivo->tm_hour > 23) {
                                            printf(RED"error"RESET": inserire un valore valido per l'ora.\n");
                                            printf("\t\tora: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tminuti: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_min) != 1 || data_arrivo->tm_min < 0 ||
                                            data_arrivo->tm_min > 59) {
                                            printf(RED"error"RESET": inserire un valore valido per i minuti.\n");
                                            printf("\t\tminuti: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\n\tPrezzo: ");
                                    while (scanf("%f", &prezzo) != 1) {
                                        printf(RED"error"RESET": inserire un valore valido per il prezzo.\n");
                                        printf("\t\tprezzo: ");
                                        fflush(stdin);
                                    }
                                    data_arrivo->tm_year-=1900;
                                    data_partenza->tm_year-=1900;
                                    data_partenza->tm_mon--;
                                    data_arrivo->tm_mon--;
                                    time_data_arrivo=mktime(data_arrivo);
                                    time_data_partenza=mktime(data_partenza);
                                    data_arrivo->tm_year+=1900;
                                    data_partenza->tm_year+=1900;
                                    data_partenza->tm_mon++;
                                    data_arrivo->tm_mon++;
                                    if(difftime(time_data_arrivo,time_data_partenza)<=0) {
                                        printf(RED"error"RESET": la data partenza deve necessariamente essere precedente alla data arrivo\n");
                                        mypause();
                                    }
                                }while(difftime(time_data_arrivo,time_data_partenza)<=0);
                                addFlight(cities_graph,prezzo,data_partenza,data_arrivo,src,dest,&err);
                                if(err==0){
                                    printf(GREEN"\nIl volo da %s a %s e' stato aggiunto correttamente nel sistema!\n"RESET, src, dest);
                                    printf("\nSi vuole aggiungere un nuovo volo? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if(scelta =='n')
                                        err = 0;
                                    else
                                        err=1;

                                }else{
                                    printf("\nSi vuole riprovare ad aggiungere un nuovo volo? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if(scelta =='n')
                                        err=0;
                                }
                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);

                            break;
                        }

                        case 4: {
                            //l'operatore ha scelto di rimuovere un volo
                            do{
                                err=0;
                                char src[MAXNAME],
                                        dest[MAXNAME],
                                        scelta;
                                int src_pos,dest_pos;
                                float prezzo;
                                struct tm *data_partenza=malloc(sizeof(struct tm));
                                struct tm *data_arrivo=malloc(sizeof(struct tm));
                                data_partenza->tm_isdst=-1;
                                data_arrivo->tm_isdst=-1;
                                data_partenza->tm_sec=0;
                                data_arrivo->tm_sec=0;
                                time_t time_data_partenza, time_data_arrivo;
                                do {
                                    system("cls");
                                    printf("----------------\n");
                                    printf(YELLOW" CANCELLAZIONE VOLO\n"RESET);
                                    printf("----------------\n");
                                    printf("\nInserire i dati del volo che si desidera eliminare:");
                                    printf("\n\n\tCitta' di partenza: ");
                                    takeSaneString(src, MAXNAME);
                                    printf("\n\tCitta' di arrivo: ");
                                    takeSaneString(dest, MAXNAME);
                                    printf("\n\tData partenza:\n");
                                    printf("\t\tgiorno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_mday) != 1 || data_partenza->tm_mday <= 0 ||
                                            data_partenza->tm_mday > 31) {
                                            printf(RED"error"RESET": inserire un valore valido per il giorno.\n");
                                            printf("\t\tgiorno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tmese: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_mon) != 1 || data_partenza->tm_mon < 1 ||
                                            data_partenza->tm_mon > 12) {
                                            printf(RED"error"RESET": inserire un valore valido per il mese.\n");
                                            printf("\t\tmese: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tanno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_year) != 1 ||
                                            data_partenza->tm_year < 2020) {
                                            printf(RED"error"RESET": inserire un valore valido per l'anno.\n");
                                            printf("\t\tanno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tora: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_hour) != 1 || data_partenza->tm_hour < 0 ||
                                            data_partenza->tm_hour > 23) {
                                            printf(RED"error"RESET": inserire un valore valido per l'ora.\n");
                                            printf("\t\tora: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tminuti: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_partenza->tm_min) != 1 || data_partenza->tm_min < 0 ||
                                            data_partenza->tm_min > 59) {
                                            printf(RED"error"RESET": inserire un valore valido per i mini.\n");
                                            printf("\t\tminuti: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;

                                    printf("\n\tData arrivo:\n");
                                    printf("\t\tgiorno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_mday) != 1 || data_arrivo->tm_mday <= 0 ||
                                            data_arrivo->tm_mday > 31) {
                                            printf(RED"error"RESET": inserire un valore valido per il giorno.\n");
                                            printf("\t\tgiorno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tmese: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_mon) != 1 || data_arrivo->tm_mon < 1 ||
                                            data_arrivo->tm_mon > 12) {
                                            printf(RED"error"RESET": inserire un valore valido per il mese.\n");
                                            printf("\t\tmese: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tanno: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_year) != 1 || data_arrivo->tm_year < 2020) {
                                            printf(RED"error"RESET": inserire un valore valido per l'anno.\n");
                                            printf("\t\tanno: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tora: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_hour) != 1 || data_arrivo->tm_hour < 0 ||
                                            data_arrivo->tm_hour > 23) {
                                            printf(RED"error"RESET": inserire un valore valido per l'ora.\n");
                                            printf("\t\tora: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\t\tminuti: ");
                                    while (err == 0) {
                                        if (scanf("%d", &data_arrivo->tm_min) != 1 || data_arrivo->tm_min < 0 ||
                                            data_arrivo->tm_min > 59) {
                                            printf(RED"error"RESET": inserire un valore valido per i minuti.\n");
                                            printf("\t\tminuti: ");
                                            fflush(stdin);
                                        } else
                                            err = 1;
                                    }
                                    err = 0;
                                    printf("\n\tPrezzo: ");
                                    while (scanf("%f", &prezzo) != 1) {
                                        printf(RED"error"RESET": inserire un valore valido per il prezzo.\n");
                                        printf("\t\tprezzo: ");
                                        fflush(stdin);
                                    }
                                    data_arrivo->tm_year-=1900;
                                    data_partenza->tm_year-=1900;
                                    data_partenza->tm_mon--;
                                    data_arrivo->tm_mon--;
                                    time_data_arrivo=mktime(data_arrivo);
                                    time_data_partenza=mktime(data_partenza);
                                    data_arrivo->tm_year+=1900;
                                    data_partenza->tm_year+=1900;
                                    data_partenza->tm_mon++;
                                    data_arrivo->tm_mon++;
                                    if(difftime(time_data_arrivo,time_data_partenza)<=0) {
                                        printf(RED"error"RESET": la data partenza deve necessariamente essere precedente alla data arrivo\n");
                                        mypause();
                                    }
                                }while(difftime(time_data_arrivo,time_data_partenza)<=0);
                                src_pos=findArrayPosition(cities_graph, src);
                                dest_pos=findArrayPosition(cities_graph, dest);
                                volo *flights_list=getFlightList(cities_graph,src_pos,dest_pos);
                                volo *volo_da_rimuovere=getFlight(flights_list,prezzo,data_partenza,data_arrivo,src,dest);
                                mypause();
                                removeFlight(cities_graph, volo_da_rimuovere, &err);
                                if(err==0){
                                    printf(GREEN"\nIl volo da %s a %s e' stato rimosso correttamente nel sistema!\n"RESET, src, dest);
                                    printf("\nSi vuole rimuovere un altro volo? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if(scelta =='n')
                                        err = 0;
                                    else
                                        err=1;

                                }else{
                                    printf("\nSi vuole riprovare a rimuovere un altro volo? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if(scelta =='n')
                                        err=0;
                                }
                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);

                            break;
                        }

                        case 5: {
                            char scelta, new_city[MAXNAME];

                            do {
                                system("cls");
                                printf("------------------\n");
                                printf(YELLOW" INSERIMENTO CITTA'\n"RESET);
                                printf("------------------\n");
                                err= 0;
                                //l'operatore ha scelto di aggiungere una citta'

                                printf("\nInserire il nome della citta' che si desidera inserire: ");

                                takeSaneString(new_city, MAXNAME);
                                cities_graph = addVertex(cities_graph, new_city, &err);

                                if (err == 0) {
                                    printf(GREEN"\nCitta' inserita correttamente nel sistema!"RESET);
                                    printf("\nSi vuole aggiungere un'altra citta'? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if (scelta == 'n'){
                                        err = 0;
                                    }
                                    else{

                                        err = 1;
                                    }

                                } else {
                                    printf("\nSi vuole riprovare ad aggiungere un'altra citta'? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if (scelta == 'n'){
                                        err = 0;
                                    }else{

                                        err=1;
                                    }

                                }

                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);
                            system("cls");

                            break;
                        }

                        case 6: {
                            //l'operatore ha scelto di rimuovere una citta'
                            char scelta, to_remove[MAXNAME];

                            do {
                                system("cls");
                                printf("------------------\n");
                                printf(YELLOW" CANCELLAZIONE CITTA'\n"RESET);
                                printf("------------------\n");
                                err= 0;
                                //l'operatore ha scelto di aggiungere una citta'

                                printf("\nInserire il nome della citta' che si desidera rimuovere: ");

                                takeSaneString(to_remove, MAXNAME);
                                cities_graph = deleteVertex(cities_graph, to_remove, &err);
                                int src_pos= findArrayPosition(cities_graph, to_remove);
                                for(int i=0;i<cities_graph->num_vertices;i++){
                                    char temp_dest[MAXNAME];
                                    strcpy(temp_dest, cities_graph->vertice[i]->nome);
                                    removeRouteFile(to_remove,temp_dest);
                                    removeRouteFile(temp_dest,to_remove);
                                    adjNode *adj_list=cities_graph->vertice[i]->destinations;
                                    if(i!=src_pos){
                                        while(adj_list){
                                            volo *flight_list=adj_list->flights_list;
                                            while(flight_list){
                                                if(strcmp(flight_list->luogo_arrivo,to_remove)==0)
                                                    removeFlightFile(flight_list);
                                                flight_list=flight_list->next;
                                            }
                                            adj_list=adj_list->next;
                                        }
                                    }else {
                                        while (adj_list) {
                                            volo *flight_list = adj_list->flights_list;
                                            while (flight_list) {
                                                if (strcmp(flight_list->luogo_arrivo, to_remove) == 0)
                                                    removeFlightFile(flight_list);
                                                flight_list = flight_list->next;
                                            }
                                            adj_list = adj_list->next;
                                        }
                                    }
                                }

                                if (err == 0) {
                                    printf(GREEN"\nCitta' rimossa correttamente dal sistema!"RESET);
                                    printf("\nSi vuole rimuovere un'altra citta'? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if (scelta == 'n'){
                                        err = 0;
                                    }
                                    else{

                                        err = 1;
                                    }

                                } else {
                                    printf("\nSi vuole riprovare a rimuovere un'altra citta'? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                    takeAnswer(&scelta);
                                    if (scelta == 'n'){
                                        err = 0;
                                    }else{

                                        err=1;
                                    }
                                }

                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);
                            system("cls");

                            break;
                        }
                            //ok case 7
                        case 7: {
                            //l'operatore ha scelto di visualizzare le tratte aeree
                            system("cls");
                            printf("------------------\n");
                            printf(YELLOW" TRATTE\n"RESET);
                            printf("------------------\n");
                            printf("\nLe tratte disponibili sono:\n\n");
                            printf(YELLOW"DA\t\t\tA\t\t\tDISTANZA\n\n"RESET);
                            printRoutes(cities_graph);
                            printf("\n");
                            mypause();
                            break;
                        }
                            //ok case 8
                        case 8: {
                            //l'operatore ha scelto di visualizzare i voli su una tratta
                            paths *possible_paths=NULL;
                            flight_path *possible_travels;
                            do{
                                err=0;
                                system("cls");
                                printf("------------------\n");
                                printf(YELLOW" VOLI\n"RESET);
                                printf("------------------\n");
                                char src[MAXNAME],
                                        dest[MAXNAME];
                                char scelta;
                                int distance=0;

                                printf("\nInserire i dati della tratta di cui si voglio visualizzare i voli:");
                                printf("\n\n\tCitta' di partenza: ");
                                takeSaneString(src, MAXNAME);
                                printf("\n\tCitta' di arrivo: ");
                                takeSaneString(dest, MAXNAME);
                                possible_paths = possiblePaths(cities_graph, src, dest);
                                possible_travels = findFlightSolutions(cities_graph, possible_paths);

                                if(possible_travels){
                                    system("cls");
                                    printf(GREEN"\tVOLI DISPONIBILI PER LA TRATTA: %s - %s\n"RESET, src, dest);
                                    printf("--------------------------------------------------------------------\n");
                                    printf(YELLOW"COD.\tDATA PARTENZA\tORARIO\t\t\tDATA ARRIVO\tORARIO\t\t\tPREZZO(euro)\n\n"RESET);
                                    printFlightsPath(possible_travels, 1);
                                }else {
                                    printf("\n\t\tSpiacenti! Non sono disponibili voli per la combinazione richiesta\n"RESET);
                                }
                                printf("\nSi vuole visualizzare i voli su una nuova tratta ? [ "GREEN"s"RESET" | "RED"n"RESET" ] ----> ");
                                takeAnswer(&scelta);
                                if(scelta =='n')
                                    err = 0;
                                else
                                    err=1;

                            }while(err==1);

                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 10);
                            break;
                        }
                            //ok case 9
                        case 9: {
                            //l'operatore ha scelto di visualizzare le citta' su cui la compagnia aerea esercita
                            system("cls");
                            printf("------------------\n");
                            printf(YELLOW" CITTA'\n"RESET);
                            printf("------------------\n");
                            printf("\nLe citta' disponibili sono:\n\n");
                            printCities(cities_graph);
                            printf("\n");
                            mypause();
                            break;
                        }
                            //ok case 10
                        case 10: {
                            //l'operatore ha scelto di visualizzare i clienti registrati
                            system("cls");
                            printf("------------------\n");
                            printf(YELLOW" CLIENTI\n"RESET);
                            printf("------------------\n");
                            printf("\nI dati dei clienti registrati sono:\n");
                            printClients(tree_client);
                            printf("\n");
                            mypause();
                            break;
                        }

                        case 0: {
                            //l'operatore ha scelto di effettuare il logout
                            strcpy(message, "\nA presto!\nLogout in corso... ");
                            caricamento(message, 10);
                            uscita=1;
                            break;
                        }

                        default: {
                            //l'operatore ha cliccato un tasto non valido
                            printf(RED"error"RESET": e' stata inserita una scelta non valida...\n");
                            mypause();
                            break;
                        }
                    }
                } while (uscita == 0);

                break;
            }

            case 3 :{
                //si e' loggato un guest
                system("cls");
                int scelta_menu = 0;
                int uscita = 0;

                do {
                    menuGuest();
                    fflush(stdin);
                    if (scanf("%d", &scelta_menu) != 1) {
                        scelta_menu = -1;
                    }

                    switch (scelta_menu) {
                        case 1: {
                            //il guest ha scelto di cercare la disponibilita' di un volo
                            int choice = 0;
                            char src[MAXNAME],
                                    dest[MAXNAME];
                            int *reachable_nodes = NULL;
                            struct tm *fascia_oraria = NULL;
                            int most_pop = 0, cheapest = 0;
                            char most_pop_city[MAXNAME],
                                    cheapest_city[MAXNAME];
                            paths *possible_paths = NULL;
                            flight_path *possible_travels = NULL;

                            fascia_oraria = malloc(sizeof(struct tm));
                            fascia_oraria->tm_isdst = -1;
                            fascia_oraria->tm_sec = 0;
                            fascia_oraria->tm_min = 0;

                            do {
                                system("cls");
                                printf("------------\n");
                                printf(YELLOW" RICERCA VOLO\n"RESET);
                                printf("------------\n");
                                printf("\nInserire l'aeroporto di partenza: ");
                                takeSaneString(src, MAXNAME);
                                if (findArrayPosition(cities_graph, src) == -1) {
                                    printf(RED"error"RESET": la citta' inserita non e' disponibile\n");
                                    mypause();
                                }
                            } while (findArrayPosition(cities_graph, src) == -1);

                            reachable_nodes = reachableNodes(cities_graph, src);
                            most_pop = findMostPopularDestination(cities_graph, reachable_nodes);
                            cheapest = findCheapestDestination(cities_graph, reachable_nodes, src);
                            strcpy(most_pop_city, findNameVertex(cities_graph, most_pop));
                            strcpy(cheapest_city, findNameVertex(cities_graph, cheapest));

                            do {
                                system("cls");
                                printf("-------------\n");
                                printf(YELLOW" RICERCA VOLO\n"RESET);
                                printf("-------------\n");
                                printf("\nHai scelto di partire da %s", src);
                                printf(YELLOW"\n\nCaro cliente, ti suggeriamo %s che e' la meta piu' economica partendo da %s"RESET,
                                       cheapest_city, src);
                                printf(YELLOW"\nPotrebbe interessarti la meta piu' gettonata che e' %s, approfittane!\n"RESET,
                                       most_pop_city);
                                printf("\nInserire l'aeroporto di arrivo: ");
                                takeSaneString(dest, MAXNAME);
                                if (findArrayPosition(cities_graph, dest) == -1) {
                                    printf(RED"error"RESET": la citta' inserita non e' disponibile, riprova\n");
                                    mypause();
                                }
                            } while (findArrayPosition(cities_graph, dest) == -1);

                            printf("\nDati per la partenza:\n");

                            do {
                                printf("\n\tgiorno: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_mday) != 1 || fascia_oraria->tm_mday > 31 ||
                                    fascia_oraria->tm_mday <= 0) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per il giorno!");
                                } else {
                                    uscita = 1;
                                }
                            } while (uscita != 1);
                            uscita = 0;
                            do {
                                printf("\n\tmese: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_mon) != 1 || fascia_oraria->tm_mon > 12 ||
                                    fascia_oraria->tm_mon <= 0) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per il mese! riprova");
                                } else {
                                    uscita = 1;
                                }
                            } while (uscita != 1);
                            uscita = 0;
                            do {
                                printf("\n\tanno: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_year) != 1 || fascia_oraria->tm_year < 2020) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per l'anno! riprova");
                                } else {
                                    uscita = 1;
                                }
                            } while (uscita != 1);
                            uscita = 0;
                            do {
                                printf("\n\tfascia oraria: ");
                                fflush(stdin);
                                if (scanf("%d", &fascia_oraria->tm_hour) != 1 || fascia_oraria->tm_hour < 0 ||
                                    fascia_oraria->tm_hour > 23) {
                                    printf(RED"\nerror"RESET": inserire un valore corretto per la fascia oraria!");
                                } else {
                                    uscita = 1;
                                }
                            } while (uscita != 1);
                            uscita = 0;

                            possible_paths = possiblePaths(cities_graph, src, dest);
                            possible_travels = makeFlightSolutions(cities_graph, possible_paths, fascia_oraria);
                            system("cls");
                            printf("------------\n");
                            printf(YELLOW" RICERCA VOLO\n"RESET);
                            printf("------------\n");
                            printf(GREEN"\tVOLI DISPONIBILI PER LA TRATTA: %s - %s\n"RESET, src, dest);
                            printf("--------------------------------------------------------------------\n");
                            printf(YELLOW"COD.\tDATA PARTENZA\tORARIO\t\t\tDATA ARRIVO\tORARIO\t\t\tPREZZO(euro)\n\n"RESET);
                            printFlightsPath(possible_travels, 1);
                            printf("\n");
                            if(possible_travels){
                                printf("\nPer acquistare un volo e' necessario essere registrati.");
                                printf("\nVuoi procedere alla registrazione come cliente o vuoi tornare al menu' principale?");
                                printf("\n[ "GREEN"1=Effettua registrazione"RESET" | "YELLOW"0=Torna al menu' principale"RESET" ]");

                                do {
                                    printf("\n\nINSERISCI QUI LA TUA SCELTA ---> ");
                                    fflush(stdin);
                                    if (scanf("%d", &choice) != 1) {
                                        printf(RED"\nerror"RESET": inserire 1 o 0 a seconda della scelta che si vuole effettuare!");
                                    } else {
                                        if (choice < 0 || choice > 1) {
                                            printf(RED"\nerror"RESET": inserire 1 o 0 a seconda della scelta che si vuole effettuare!");
                                        } else {
                                            uscita = 1;
                                        }
                                    }
                                } while (uscita != 1);

                                if (choice == 1) {

                                    strcpy(message, "\nPerfetto!\nSi verra' indirizzati alla pagina di registrazione... ");
                                    caricamento(message, 10);
                                    system("cls");
                                    signupUser(tree_client);
                                    uscita=1;

                                }
                            }else
                                printf("\n\t\tSpiacenti! Non sono disponibili voli per la combinazione richiesta\n"RESET);


                            strcpy(message, "\nSto tornando al menu' principale... ");
                            caricamento(message, 13);
                            break;
                        }

                        case 0: {
                            //il guest ha deciso di effettuare il logout
                            strcpy(message, "\nGrazie per aver utilizzato il programma!\nLogout in corso... ");
                            caricamento(message, 10);
                            uscita = 1;
                            break;
                        }

                        default: {
                            //il cliente ha cliccato un tasto non valido
                            printf(RED"error"RESET": e' stata inserita una scelta non valida...\n");
                            mypause();
                            break;
                        }
                    }
                } while (uscita == 0);
                break;
            }

            case 0 :{
                log_in_value=0;
                strcpy(message, "\nGrazie per aver utilizzato il programma! Uscita in corso ");
                caricamento(message, 10);
                break;
            }

            default :{
                printf(RED"error"RESET": e' stata inserita una scelta non valida...\n");
                mypause();
                break;

            }
        }

    }while(log_in_value!=0);

    return 0;
}
//ritorna: 0 se ordina l' uscita; 1 se ha loggato un cliente; 2 un operatore; 3 un guest
int login(cliente *tree_cliente, operatore *tree_operator, void **logged_user) {
    char scelta;
    int scelta_login;
    char message[100];
    char nome_utente[MAXEMAIL];
    char password[13];
    cliente *client = NULL;
    operatore *operator = NULL;

    do {
        system("cls");
        printf("\n----------------------------------------------------------------------\n");
        printf(GREEN"BENVENUTO NEL PROGRAMMA GESTIONALE DELLA COMPAGNIA AEREA FRANZOFLIGHT\n"RESET);
        printf("----------------------------------------------------------------------\n\n");

        scelta_login=0;
        scelta='\0';
        client=NULL;
        operator=NULL;

        printf("\t[1] LOGIN CLIENTI\n\t[2] LOGIN OPERATORI\n\t[3] ACCEDI COME GUEST\n\t[4] EFFETTUA REGISTRAZIONE\n\t[0] ESCI DAL PROGRAMMA");
        printf("\n\nINSERISCI QUI LA TUA SCELTA ->");


        if(scanf("%d", &scelta_login)!=1){
            scelta_login=-1;
        }

        switch (scelta_login) {
            case 0: {
                return 0;
            }

            case 1: {
                do {
                    system("cls");
                    printf("---------------\n");
                    printf(YELLOW" LOGIN CLIENTI\n"RESET);
                    printf("---------------\n");
                    printf("Nome utente: ");
                    takeSaneString(nome_utente, sizeof(nome_utente));
                    printf("Password: ");
                    insertPassword(password, sizeof(password));

                    client = getClient(tree_cliente, nome_utente);
                    if (client) {
                        if (strcmp(client->password, password) == 0) {
                            system("cls");
                            printf("Piacere di rivederti %s! Bentornato!\n", client->nome);
                            strcpy(message,"Sto caricando il menu' principale  ");
                            caricamento(message,13);
                            *logged_user = client;
                            return 1;
                        } else {
                            printf("Password errata\n");
                            printf("Si intende riprovare il login?\n[s/n] -> ");
                            takeAnswer(&scelta);
                            if(scelta=='s')
                                client=NULL;
                        }
                    } else {
                        printf(RED"\n\nerror"RESET":L'utente inserito non risulta registrato. Si vuole registrarlo?\n[s/n] -> ");
                        takeAnswer(&scelta);
                        if (scelta == 's') {
                            //l'utente ha deciso di registrarsi
                            system("cls");
                            tree_cliente = signupUser(tree_cliente);
                            strcpy(message,"\n\nSi verra' reindirizzati alla pagina di login clienti  ");
                            caricamento(message,13);
                            client=NULL;
                        }
                    }
                }while(client==NULL && scelta=='s');
                break;
            }

            case 2 : {
                do{
                    system("cls");
                    printf("-----------------\n");
                    printf(YELLOW" LOGIN OPERATORE\n"RESET);
                    printf("-----------------\n");
                    printf("ID operatore: ");
                    takeSaneString(nome_utente,sizeof(nome_utente));
                    printf("Password: ");
                    insertPassword(password, sizeof(password));
                    operator = getOperatore(tree_operator, nome_utente);

                    if (operator) {
                        if(strcmp(operator->password,password)==0) {
                            system("cls");
                            printf("Piacere di rivederti %s! Buon lavoro!\n", operator->id_operator);
                            strcpy(message,"Sto caricando il menu' gestionale  ");
                            caricamento(message,13);
                            *logged_user=operator;
                            return 2;
                        }else{
                            printf(RED"error"RESET": password errata\n");
                            printf("Si intende riprovare il login?\n[s/n] -> ");
                            takeAnswer(&scelta);
                            if(scelta=='s')
                                operator=NULL;
                        }
                    }else{
                        printf(RED"\n\nerror"RESET": L'operatore inserito non risulta essere registrato. Contattare l'amministrazione!\n");
                        printf("\nSi intende riprovare il login?\n[s/n] -> ");
                        takeAnswer(&scelta);
                    }
                }while(operator==NULL && scelta=='s');
                break;
            }

            case 3 : {
                system("cls");
                printf("----------------\n");
                printf(YELLOW" ACCESSO GUEST\n"RESET);
                printf("----------------\n");
                printf(YELLOW"warning"RESET": Non sara' possibile effettuare acquisti senza essersi loggati ma si potranno visionare i voli\n");
                mypause();
                return 3;
            }

            case 4: {
                system("cls");
                tree_cliente= signupUser(tree_cliente);
                strcpy(message, YELLOW"\nSi verra reindirizzari alla pagina principale\nper poter effettuare altre operazioni  "RESET);
                caricamento(message,13);
                break;
            }

            default: {
                printf(RED"error"RESET": e' stata inserita una scelta non valida...\n");
                mypause();
                break;
            }
        }

    }while ( scelta_login != 0);

    return scelta_login;
}

void menuClient(char *nome, char *cognome, char *e_mail, int punti){
    system("cls");
    printf("\n");
    printf("\t\t\t\t\t\t"RESET"cliente : "GREEN"%s"RESET, e_mail);
    printf("\n\t\t\t\t\t\t"RESET"saldo punti : "GREEN"%d"RESET, punti);
    printf("\n\n");
    printf(YELLOW"    * * * * * * * * * * * * * * * * * "RESET);
    printf(CYAN"MENU "RESET);
    printf(YELLOW"* * * * * * * * * * * * * * * * * *\n");
    printf("    *\t\t\t\t\t\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[1]"RESET" Acquista/Ricerca volo\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[2]"RESET" Visualizza destinazioni disponibili\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[3]"RESET" Visualizza prenotazioni attive\t\t     ");
    printf(YELLOW"*\n");
    printf("    *\t\t\t\t\t\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[0]"RESET" Logout");
    printf(YELLOW"\t\t\t\t             *"CYAN"\n\n\t\t\tINSERISCI QUI LA TUA SCELTA->"RESET);

}

void menuGuest(){
    system("cls");
    printf("\n");
    printf("\t\t\t\t\t\t   accesso eseguito come guest");
    printf("\n\n");
    printf(YELLOW"    * * * * * * * * * * * * * * * * * "RESET);
    printf(CYAN"MENU "RESET);
    printf(YELLOW"* * * * * * * * * * * * * * * * * *\n");
    printf("    *\t\t\t\t\t\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[1]"RESET" Cerca disponibilita' voli\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *\t\t\t\t\t\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[0]"RESET" Logout");
    printf(YELLOW"\t\t\t\t             *"CYAN"\n\n\t\t\tINSERISCI QUI LA TUA SCELTA->"RESET);
}

void menuOperator(char *id_operator){
    system("cls");
    printf("\n");
    printf("\t\t\t\t\t\t\t"RESET"operatore : "GREEN"%s"RESET, id_operator);
    printf("\n\n");
    printf(YELLOW"    * * * * * * * * * * * * * * * * * "RESET);
    printf(CYAN"MENU "RESET);
    printf(YELLOW"* * * * * * * * * * * * * * * * * *\n");
    printf("    *\t\t\t\t\t\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[1]"RESET"  Aggiungi nuova tratta\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[2]"RESET"  Rimuovi una tratta esistente\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[3]"RESET"  Aggiungi nuovo volo\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[4]"RESET"  Rimuovi volo\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[5]"RESET"  Aggiungi citta'\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[6]"RESET"  Rimuovi citta'\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[7]"RESET"  Visualizza tratte\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[8]"RESET"  Visualizza voli\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[9]"RESET"  Visualizza citta'\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[10]"RESET" Visualizza clienti\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *\t\t\t\t\t\t\t\t\t     ");
    printf(YELLOW"*\n");
    printf("    *   "CYAN"\t\t[0]"RESET" Logout");
    printf(YELLOW"\t\t\t\t             *"CYAN"\n\n\t\t\tINSERISCI QUI LA TUA SCELTA->"RESET);
}