#ifndef PROGETTOII_PATHLIST_H
#define PROGETTOII_PATHLIST_H
#include "utility.h"

typedef struct path{
    int key;
    int distance; //la distanza minima per questo nodo
    int prev_vertex; //la chiave del nodo predecessore all'interno dello shortest path per questo nodo
    struct path *prev;
    struct path *next;
}path;

typedef struct paths{
    path *list;
    struct paths *next;
}paths;

//alloca memoria al nuovo nodo di tipo path
path *newNode(int key, int distance, int prev_vertex);

//inserisce in maniera ordinata rispettando la relazione d'ordine crescente sulla chiave "distance"
path *insertNode(path *path_list, int key, int distance, int prev_vertex);

//
path *insertHead(path *path_list, int key, int distance, int prev_vertex);
//rimuove un nodo con chiave "key" dalla lista
path *removeNode(path *path_list, int key);

//restituisce il riferimento ad un nodo(con chiave "key") rimosso dalla lista
path *getNRemove(path **path_list, int key);

//esegue l'operazione di rilassamento di un nodo
void relax(path **head, path **curr, int key, int new_distance, int new_prev_vertex);

//stampa la lista a schermo
void printPathList(path *path_list);

//ritorna NULL se il nodo con la chiave in ingresso non e' presente all'interno della lista
path *getNode(path *path_list, int key);

path *destroyPathList(path *path_list);

paths *destroyPathsCollection(paths *paths_list);

//aggiunge un nuovo percorso alla lista che colleziona percorsi
paths *addNewPath(paths *paths_list, path *percorso);

//stampa a schermo le liste di tipo path collezionate in paths_list
void printPathsList(paths *paths_list);

#endif //PROGETTOII_PATHLIST_H