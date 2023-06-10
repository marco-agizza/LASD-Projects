#ifndef PROGETTOII_FLIGHTSLISTS_H
#define PROGETTOII_FLIGHTSLISTS_H

#include "utility.h"
#define SIZE_PLACE 50

typedef struct volo{
    struct tm *data_partenza,
            *data_arrivo;
    char luogo_partenza[SIZE_PLACE],
            luogo_arrivo[SIZE_PLACE];
    int num_acquirenti;
    float prezzo;
    struct volo *next;
}volo;

//struttura che ci serve a collezionare una lista di voli da prendere in un viaggio da una src A ad una dest B (percorsi con scali e non)
typedef struct flight_path{
    volo *flight;
    struct flight_path *next;
}flight_path;

//inserisce un nuovo volo da una cittÃ  ad un altra
volo *insertFlight(volo *flights_list, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo);

//inserisce un nuovo volo da una citta' ad un'altra
volo *enqueueFlight(volo *flights_list, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo);

//alloca memoria per un nuovo nodo della lista
volo *newFlight(float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo);

//ritorna il numero totale di biglietti venduti per ogni volo della lista (per ogni nodo)
int getNumBuyers(volo *flights_list);

//stampa i voli sulla tratta da src a dest
void printFlightsList(volo *flights_list);

//aggiorna per riferimento cheapest_flight così che esso punti a volo effettivamente più economico di flights_list
void lowestPriceFlight(volo *flights_list, float *min_prezzo, volo **cheapest_flight);

//inserisce un nuova sequenza di voli all'interno di una collezione di sequenze di voli che portano da una src A ad una dst B (con eventuali scali)
flight_path *addNewFlightPath(flight_path *flight_collection, volo *new_flight_path);

//stampa a schermo delle possibili soluzioni per spostarsi da una src A ad una dst B
void printFlightsPath(flight_path *flight_collection, int i);

//ritorna il riferimento al volo della lista flights_list con data partenza più prossima (ma successiva) a data_arrivo in ingresso
void getExchangeFly(volo *flights_list, struct tm *data_arrivo, struct tm *prossima_data_partenza, volo **preferred_flight);

//restituisce il numero di nodi di una flight_path
int getNumTravel(flight_path *flight_collection);

//restituisce un riferimento ad una lista di voli tra quelle collezionate in flight_collection
volo *getTravel(flight_path *flight_collection, int numero_soluzione);

#endif //PROGETTOII_FLIGHTSLISTS_H