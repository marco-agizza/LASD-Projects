#ifndef PROGETTOII_ROUTEGRAPH_H
#define PROGETTOII_ROUTEGRAPH_H

#include "utility.h"
#include "flightsLists.h"
#include "pathList.h"
#include "clientBST.h"

typedef struct adjNode{
    int distanza,
            key;
    volo *flights_list;
    struct adjNode *next;
}adjNode;

typedef struct vertex{
    char nome[MAXNAME];
    adjNode  *destinations;
}vertex;

typedef struct tratta{
    int num_vertices;
    vertex **vertice;
}tratta;

//inizializza il grafo con i nomi delle citta' presso cui la nostra compagnia esercita e le rispettive tratte
tratta *initCities(void);

//aggiunge un arco tra vertici del grafo
void insertRoute(tratta *cities_graph, char *src, char *dest, int distanza);

//aggiunge un arco tra vertici del grafo e sincronizza la modifica scrivendo sul file "routes.txt"
void addRoute(tratta *cities_graph, char *src, char *dest, int distanza, int *err);

//aggiunge al file "routes.txt" una nuova tratta
void addRouteFile(char *src, char *dest, int distanza);

//stampa il grafo delle citta'
void printCities(tratta *cities_graph);

//stampa tutte le tratte disponibili
void printRoutes(tratta *cities_graph);

//stampa la lista di adiacenza di un vertice
void printAdjNodes(tratta *cities_graph, adjNode *adj_list);

//alloca in memoria un nodo per la lista di adiacenza di un vertice del grafo
adjNode *createNode(int key, int distanza);

//elimina una nodo di una lista
adjNode *deleteNode(tratta *cities_graph, adjNode *adj_list, int key);

//dealloca la lista di voli dalla memoria
//void destroyFlights(tratta *cities_graph, volo **flights_list);
volo *destroyFlights(tratta *cities_graph, volo *flights_list);

//rimuove una tratta tra due citta'
void removeRoute(tratta *cities_graph, char *src, char *dest, int *err);

//elimina una tratta nel file "routes.txt"
void removeRouteFile(char *src, char *dest);

//ritorna l'indice dell'array in cui la cittÃ  (char*) si trova
int findArrayPosition(tratta *cities_graph, char *city);

//ritorna il nome della citta' con indice city_pos
char *findNameVertex(tratta *cities_graph, int city_pos);

//coontrolla se la citta'  che riceve in ingresso e' gia'  registrata (ritorna 1 se e' gia'  presente)
int checkCity(tratta *cities_graph, char *city);

//rimuove dalla lista di adiacenza in ingresso le occorrenze del nodo con chiave key
adjNode *checkRicorrenzeRemoval(adjNode *adj_list, int key);

//dealloca dalla memoria i nodi della lista in ingresso
void destroyAdjNodes(tratta *cities_graph, adjNode **adj_list);

//rimuove un vertice del grafo
tratta *deleteVertex(tratta *cities_graph, char *city, int *err);

//aggiunge una citta'  al grafo
tratta *addVertex(tratta *cities_graph, char *city, int *err);

//aggiunge una citta'  al file
void addCitiesFile(char *city);

//rimuove una citta'  dal file
void deleteCitiesFile(int nVertici, char *city);

//rimuove un volo dalla lista
volo *deleteFlight(volo *flights_list, volo *fly);

//inizializza la lista dei voli per ogni tratta
void initFlights(tratta *cities_graph);

//restituisce il nodo di adiacenza al vertice src_pos del grafo con chiave dest_pos
adjNode *getDestination(tratta *cities_graph, int src_pos, int dest_pos);

//restituisce la lista dei voli su una tratta verso una citta' con chiave key
adjNode *findDestination(adjNode *adj_list, int key);

//aggiunge un nuovo volo da una citta' all'altra sincronizzando il tutto attraverso un file
void addFlight(tratta *cities_graph, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo, int *err);

//aggiunge un nuovo volo al file che li colleziona (flights_list.txt)
void addFlightFile(volo *fly);

//stampa la lista di voli che percorrono la tratta tra src e dest
void printFlightsRoute(tratta *cities_graph, char *src, char *dest);

//ritorna NULL se il volo in ingresso non è già presente tra i voli verso l'adj-node in ingresso
volo *getFlight(volo *flight_list, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo);

//ritorna la lista dei voli tra una citta' src e una dest
volo *getFlightList(tratta *cities_graph, int src_pos, int dest_pos);

//rimuove il volo in ingresso dalla lista dei voli sulla tratta src-dest
void removeFlight(tratta *cities_graph, volo *fly, int *err);

//rimuove il volo in ingresso dal file
void removeFlightFile(volo *fly);

//ritorna il percorso minimo da src a dest sotto forma di array di interi (ogni intero identifica la chiave di un vertice del grafo)
path *shortestDistancePath(tratta *cities_graph, char *src, char *dest);

//scandisce la lista di adiacenza in ingresso e inserisce/rilassa ogni suo nodo all'interno di path_list
path *scanAdjNodes(adjNode *adj_list, path *path_list, path *shortest_path, int src_vertex);

//ritorna il percorso meno costoso per raggiungere dest a partire da src
path *lowestPricePath(tratta *cities_graph, char *src, char *dest);

//scandisce la lista di adiacenza in ingresso e inserisce/rilassa ogni suo nodo all'interno di path_list
path *scanAdjNodesPrice(adjNode *adj_list, path *path_list, path *shortest_path, int src_vertex);

//restituisce la lista di percorsi possibili per raggiungere dest partendo da src
paths *possiblePaths(tratta *cities_graph, char *src, char *dest);

//scorrimento DFS a sostegno della funzione possiblePath
void findPossiblePath(tratta *cities_graph, int* visited,int src_pos, int dest_pos, path *path_list, paths **paths_list);

//costruisce una lista di liste di voli che rappresentano tutte le possibili soluzioni con cui viaggiare da una src ad una dst dalla fascia oraria in ingresso in poi (fino alla mezzanotte)
flight_path *makeFlightSolutions(tratta *cities_graph, paths *paths_collection, struct tm *fascia_oraria);

flight_path *makeFlightsPath(tratta *cities_graph, flight_path *flights_path, path *path_list, struct tm *fascia_oraria);

void makeNewFlightSolution(tratta *cities_graph, volo **new_solution, struct tm *data_arrivo, path *new_source, path *new_dest, int *err);

int *reachableNodes(tratta *cities_graph, char *src);

void DFSSearch(tratta *cities_graph, int *visited, int src_pos);

int *findNumBuyers(tratta *cities_graph, int *reachable_nodes);

void increaseBuyers(adjNode *adj_list, int *reachable_nodes, int *num_buyers, int size);

float *findLowestPrice(tratta *cities_graph, char *src, int *reachable_nodes);

int findMostPopularDestination(tratta *cities_graph, int *reachable_nodes);

int findCheapestDestination(tratta *cities_graph, int *reachable_nodes, char *src);

biglietti *updateListaBiglietti(tratta *cities_graph, cliente **my_client, biglietti *ticket_list);

flight_path *findFlightSolutions(tratta *cities_graph, paths *paths_collection);

flight_path *findFlightsPath(tratta *cities_graph, flight_path *flights_path, path *path_list);

void printPathCities(tratta *cities_graph, path *path_list);

#endif //PROGETTOII_ROUTEGRAPH_H