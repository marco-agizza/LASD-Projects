#include "routeGraph.h"

tratta *initCities(void){
    int num_vertices, distance;
    char temp[MAXNAME], source[MAXNAME], destination[MAXNAME];
    char src_temp[MAXNAME], dest_temp[MAXNAME];
    FILE *fp=NULL;

    tratta *grafo=malloc(sizeof(tratta));
    if((fp= fopen("cities.txt", "r"))==NULL){
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }
    fscanf(fp,"%d",&num_vertices);
    grafo->num_vertices= num_vertices;
    grafo->vertice=malloc(num_vertices*sizeof(vertex*));
    for(int i=0; i<num_vertices; i++){
        grafo->vertice[i]=malloc(sizeof(vertex));
        fscanf(fp, "%s", temp);
        for(int j = 0;j<=strlen(temp);j++){
            if(temp[j] == '_'){
                temp[j] = ' ';
            }
        }
        strcpy(grafo->vertice[i]->nome,temp);
        grafo->vertice[i]->destinations=NULL;
    }
    fclose(fp);
    if((fp= fopen("routes.txt", "r"))==NULL){
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }
    while(!feof(fp)){
        fscanf(fp, "%s  %s  %d",source, destination, &distance);
        strcpy(src_temp, source);
        strcpy(dest_temp, destination);
        for(int j = 0;j<=strlen(src_temp);j++){
            if(src_temp[j] == '_'){
                src_temp[j] = ' ';
            }
        }
        for(int j = 0;j<=strlen(dest_temp);j++){
            if(dest_temp[j] == '_'){
                dest_temp[j] = ' ';
            }
        }
        insertRoute(grafo, src_temp, dest_temp, distance);
    }
    fclose(fp);
    return grafo;
}

void printCities(tratta *cities_graph){
    for(int i=0; i<cities_graph->num_vertices; i++) {
        printf("- "GREEN"%s"RESET" con volo diretto verso   \t", cities_graph->vertice[i]->nome);
        printAdjNodes(cities_graph, cities_graph->vertice[i]->destinations);
        printf("\n");
    }

}

void printRoutes(tratta *cities_graph){
    adjNode *temp= NULL;
    for(int i=0;i<cities_graph->num_vertices;i++){
        temp= cities_graph->vertice[i]->destinations;
        while(temp){
            printf("%s    \t\t%s    \t\t%d\n", cities_graph->vertice[i]->nome,
                   findNameVertex(cities_graph, temp->key), temp->distanza);
            temp= temp->next;
        }
    }
}

void printAdjNodes(tratta *cities_graph, adjNode *adj_list){
    if(cities_graph){
        if(adj_list) {
            printf("-> "YELLOW"%s"RESET, cities_graph->vertice[adj_list->key]->nome);
            printAdjNodes(cities_graph, adj_list->next);
        }
    }
}

adjNode *createNode(int key, int distanza) {
    adjNode *new_node=(adjNode*)malloc(sizeof(adjNode));
    new_node->distanza=distanza;
    new_node->key=key;
    new_node->flights_list=NULL;
    new_node->next=NULL;

    return new_node;
}

adjNode *deleteNode(tratta *cities_graph, adjNode *adj_list, int key){
    if(adj_list){
        if(adj_list->key!=key){
            adj_list->next=deleteNode(cities_graph, adj_list->next, key);
        }else{
            adjNode *temp= adj_list;
            adj_list= adj_list->next;
            adj_list->flights_list=destroyFlights(cities_graph, temp->flights_list);
            free(temp);
            temp=NULL;
        }
    }
    return adj_list;
}

volo *destroyFlights(tratta *cities_graph, volo *flights_list){
    if(cities_graph) {
        if (flights_list) {
            removeFlightFile(flights_list);
            flights_list->next=destroyFlights(cities_graph,flights_list->next);
            volo *temp=flights_list->next;
            free(flights_list->data_partenza);
            free(flights_list->data_arrivo);
            free(flights_list);
            return temp;
        }
    }
    return flights_list;
}

void insertRoute(tratta *cities_graph, char *src, char *dest, int distanza){
    if(cities_graph) {
        int dest_pos = findArrayPosition(cities_graph, dest);
        int src_pos = findArrayPosition(cities_graph, src);
        if (dest_pos != -1 && src_pos != -1) {
            adjNode *new_node = createNode(dest_pos, distanza);
            new_node->next = cities_graph->vertice[src_pos]->destinations;
            cities_graph->vertice[src_pos]->destinations = new_node;
        } else {
            printf(RED"\nerror"RESET": impossibile aggiungere una rotta tra citta' non registrate\n");
        }
    }
}

void addRoute(tratta *cities_graph, char *src, char *dest, int distanza, int *err){
    if(cities_graph) {
        *err=0;
        int dest_pos = findArrayPosition(cities_graph, dest);
        int src_pos = findArrayPosition(cities_graph, src);

        if (dest_pos != -1 && src_pos != -1) {
            if(!getDestination(cities_graph, src_pos, dest_pos)){
                    adjNode *new_node = createNode(dest_pos, distanza);
                    new_node->next = cities_graph->vertice[src_pos]->destinations;
                    cities_graph->vertice[src_pos]->destinations = new_node;
                    addRouteFile(src, dest, distanza);

            }else{
                printf(RED"\nerror"RESET": si e' tentato di aggiungere una tratta gia' esistente\n");
                *err=1;
            }
        } else {
            printf(RED"\nerror"RESET": impossibile aggiungere una rotta tra citta' non registrate\n");
            *err=1;
        }
    }
}

void addRouteFile(char *src, char *dest, int distanza){
    FILE *fPtr;
    char temp1[MAXNAME], temp2[MAXNAME];

    fflush(stdin);
    fPtr = fopen("routes.txt", "a");
    if (fPtr == NULL) {
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }
    strcpy(temp1, src);
    strcpy(temp2, dest);
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

    fprintf(fPtr, "\n%s\t%s\t%d", temp1, temp2, distanza);
    fclose(fPtr);
}

void removeRoute(tratta *cities_graph, char *src, char *dest, int *err){
    if(cities_graph) {
        *err=0;
        int dest_pos = findArrayPosition(cities_graph, dest);
        int src_pos = findArrayPosition(cities_graph, src);
        if (dest_pos != -1 && src_pos != -1) {
            if (getDestination(cities_graph, src_pos, dest_pos)) {
                cities_graph->vertice[src_pos]->destinations = deleteNode(cities_graph, cities_graph->vertice[src_pos]->destinations,dest_pos);
                removeRouteFile(src, dest);
            } else {
                *err = 1;
                printf(RED"\nerror"RESET": si e' tentato di rimuovere una tratta inesistente\n");
            }
        } else {
            *err=1;
            printf(RED"\nerror"RESET": impossibile rimuovere una rotta tra due citta' non registrate\n");
        }
    }
}

void removeRouteFile(char *src, char *dest){
    FILE *fPtr, *fTemp;
    char temp1[MAXNAME], temp2[MAXNAME];
    char source[MAXNAME], destination[MAXNAME];
    int distance;

    fflush(stdin);
    fPtr = fopen("routes.txt", "r");
    fTemp = fopen("replace.tmp", "w");
    if (fPtr == NULL || fTemp == NULL) {
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }

    while (!feof(fPtr)) {
        fscanf(fPtr, "%s %s %d", source, destination, &distance);

        strcpy(temp1, source);
        strcpy(temp2, destination);
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

        if(!(strcmp(temp1, src)==0 && strcmp(temp2, dest)==0))
            fprintf(fTemp, "%s\t%s\t%d\n", source, destination, distance);
    }

    int sz= ftell(fTemp);
    fseek(fTemp, sz, SEEK_SET);
    ftruncate(fileno(fTemp), sz-2);

    fclose(fPtr);
    fclose(fTemp);

    //elimino la vecchia versione del file e rinomino la nuova
    remove("routes.txt");
    rename("replace.tmp", "routes.txt");
}

int findArrayPosition(tratta *cities_graph, char *city){
    if(cities_graph) {
        for (int i = 0; i < cities_graph->num_vertices; i++) {
            if (strcmp(cities_graph->vertice[i]->nome, city) == 0)
                return i;
        }
    }
    return -1;
}

char *findNameVertex(tratta *cities_graph, int city_pos){
    if(cities_graph){
        if(city_pos<cities_graph->num_vertices)
            return cities_graph->vertice[city_pos]->nome;
    }
    return NULL;
}

int checkCity(tratta *cities_graph, char *city){
    if(cities_graph) {
        for (int i = 0; i < cities_graph->num_vertices; i++) {
            if (strcmp(cities_graph->vertice[i]->nome, city) == 0)
                return 1;
        }
    }
    return 0;
}

tratta *deleteVertex(tratta *cities_graph, char *city, int *err){
    *err= 0;
    if(cities_graph){
        int i= 0, x= 0;
        int city_pos= findArrayPosition(cities_graph, city);
        if(city_pos==-1){
            *err= 1;
            printf(YELLOW"warning"RESET": la citta' inserita non e' presente nel sistema.\n");
            return cities_graph;
        }

        vertex **temp= cities_graph->vertice;
        cities_graph->vertice= calloc(cities_graph->num_vertices, sizeof(vertex*));

        for(i=0;i<cities_graph->num_vertices;i++){
            if(i!=city_pos) {
                cities_graph->vertice[x]=malloc(sizeof(vertex));
                adjNode *temp_node= temp[i]->destinations;
                while(temp_node){
                    if(temp_node->key == city_pos) {
                        temp_node->flights_list = destroyFlights(cities_graph, temp_node->flights_list);
                    }
                    temp_node= temp_node->next;
                }
                cities_graph->vertice[x]->destinations= checkRicorrenzeRemoval(temp[i]->destinations, city_pos);
                strcpy(cities_graph->vertice[x]->nome,temp[i]->nome);
                x++;
            }else{
                destroyAdjNodes(cities_graph,&temp[i]->destinations);
            }
        }

        deleteCitiesFile(cities_graph->num_vertices, city);
        free(*temp);
        cities_graph->num_vertices-= 1;
    }
    return cities_graph;
}

adjNode *checkRicorrenzeRemoval(adjNode *adj_list, int key){
    if(adj_list){
        adj_list->next = checkRicorrenzeRemoval(adj_list->next, key);
        if (adj_list->key == key) {
            adjNode *temp = adj_list;
            adj_list = adj_list->next;
            free(temp);
            temp = NULL;
        }
    }
    return adj_list;
}

void destroyAdjNodes(tratta *cities_graph, adjNode **adj_list){
    if(cities_graph) {
        if (*adj_list) {
            adjNode **temp = adj_list;
            *adj_list = (*adj_list)->next;
            if(*temp)
                (*temp)->flights_list= destroyFlights(cities_graph, (*temp)->flights_list);
            free(*temp);
            destroyAdjNodes(cities_graph, adj_list);
        }
    }
}

tratta *addVertex(tratta *cities_graph, char *city, int *err){
    if(!checkCity(cities_graph,city)) {
        if (cities_graph) {
            *err= 0;
            cities_graph->num_vertices += 1;
            cities_graph->vertice = realloc(cities_graph->vertice,(cities_graph->num_vertices) * sizeof(vertex*));
            cities_graph->vertice[cities_graph->num_vertices - 1] = malloc(sizeof(vertex));
            cities_graph->vertice[cities_graph->num_vertices - 1]->destinations = NULL;
            strcpy(cities_graph->vertice[cities_graph->num_vertices - 1]->nome, city);
            fflush(stdin);
            addCitiesFile(city);
        } else {
            *err= 0;
            tratta *new_city = malloc(sizeof(tratta));
            new_city->vertice = malloc(sizeof(vertex *));
            new_city->vertice[0] = malloc(sizeof(vertex));
            new_city->vertice[0]->destinations = NULL;
            strcpy(new_city->vertice[0]->nome, city);
            new_city->num_vertices = 1;
            fflush(stdin);
            addCitiesFile(city);
            return new_city;
        }
    }else{
        *err= 1;
        printf(YELLOW"\nwarning"RESET": la citta' e' gia' registrata\n");
    }
    return cities_graph;
}

void addCitiesFile(char *city) {
    FILE *fPtr;
    int num_cities;
    char temp[MAXNAME];

    fflush(stdin);
    fPtr = fopen("cities.txt", "r+");
    if (fPtr == NULL) {
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }

    fseek(fPtr, 0, SEEK_SET);
    fscanf(fPtr, "%d", &num_cities);
    fseek(fPtr, 0, SEEK_SET);
    fprintf(fPtr, "%d", num_cities+1);

    fseek(fPtr, 0, SEEK_END);
    strcpy(temp, city);
    for(int j = 0;j<=strlen(temp);j++){
        if(temp[j] == ' '){
            temp[j] = '_';
        }
    }
    fprintf(fPtr, "\n%s", temp);
    fclose(fPtr);
}

void deleteCitiesFile(int nVertici, char *city){
    FILE *fPtr, *fTemp;
    char buffer[BUFFER_SIZE];
    int num_cities;

    fflush(stdin);
    fPtr = fopen("cities.txt", "r");
    fTemp = fopen("replace.tmp", "w");
    if (fPtr == NULL || fTemp == NULL) {
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }
    fscanf(fPtr, "%d", &num_cities);
    num_cities -= 1;

    fprintf(fTemp, "%d", num_cities);
    char tmp[MAXNAME];
    int i=-1;
    while ((fgets(buffer, BUFFER_SIZE, fPtr)) != NULL) {
        i+=1;
        for(int j = 0;j<=strlen(buffer);j++){
            if(buffer[j] == '_'){
                buffer[j] = ' ';
            }
        }
        if(i!=nVertici){
            strcpy(tmp, city);
            strcat(tmp, "\n");
            if(strcmp(buffer, tmp)!=0){
                for(int j = 0;j<=strlen(buffer);j++){
                    if(buffer[j] == ' '){
                        buffer[j] = '_';
                    }
                }
                fputs(buffer, fTemp);
            }
        }else{
            if(strcmp(buffer,city)!=0) {
                for(int j = 0;j<=strlen(buffer);j++){
                    if(buffer[j] == ' '){
                        buffer[j] = '_';
                    }
                }
                fputs(buffer, fTemp);
            }else{
                fseek(fTemp, 0, SEEK_END);
                int sz= ftell(fTemp);
                ftruncate(fileno(fTemp), sz-2);
                fputs("\0", fTemp);
            }
        }
    }

    fclose(fPtr);
    fclose(fTemp);

    //elimino la vecchia versione del file e rinomino la nuova
    remove("cities.txt");
    rename("replace.tmp", "cities.txt");
}

void initFlights(tratta *cities_graph){
    int src_pos, dest_pos;
    char src[MAXNAME], dest[MAXNAME];
    float prezzo;
    adjNode *destination= NULL;

    FILE *fp= NULL;
    if((fp= fopen("flights_list.txt", "r"))==NULL){
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }

    while(!feof(fp)){
        struct tm *data_partenza= malloc(sizeof(struct tm));
        struct tm *data_arrivo= malloc(sizeof(struct tm));
        data_partenza->tm_isdst=-1;
        data_arrivo->tm_isdst=-1;
        data_partenza->tm_sec=0;
        data_arrivo->tm_sec=0;
        fscanf(fp, "%d %d %d\t%d %d %d\t%d %d\t%d %d\t%s\t%s\t%f", &data_partenza->tm_mday, &data_partenza->tm_mon, &data_partenza->tm_year,
               &data_arrivo->tm_mday, &data_arrivo->tm_mon, &data_arrivo->tm_year, &data_partenza->tm_hour, &data_partenza->tm_min,
               &data_arrivo->tm_hour, &data_arrivo->tm_min, src, dest, &prezzo);

        src_pos= findArrayPosition(cities_graph, src);
        dest_pos= findArrayPosition(cities_graph, dest);

        if(src_pos==-1 || dest_pos==-1){
            printf(RED"\nerror"RESET": le citta' inserite non risultano registrate!\n");
            return;
        }

        destination= findDestination(cities_graph->vertice[src_pos]->destinations, dest_pos);
        if(destination){
            destination->flights_list= insertFlight(destination->flights_list, prezzo, data_partenza, data_arrivo, src, dest);
        }else{
            printf(RED"\nerror"RESET": si e' verificato un errore nell' inserimento del volo da %s a %s con partenza alle %d:%d\n", src, dest, data_partenza->tm_hour, data_partenza->tm_min);
        }
    }
    fclose(fp);
}

adjNode *getDestination(tratta *cities_graph, int src_pos, int dest_pos){
    if(cities_graph){
        return findDestination(cities_graph->vertice[src_pos]->destinations, dest_pos);
    }
    return NULL;
}

adjNode *findDestination(adjNode *adj_list, int key){
    if(adj_list){
        if(adj_list->key==key)
            return adj_list;

        return findDestination(adj_list->next, key);
    }
    return NULL;
}

void addFlight(tratta *cities_graph, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo, int *err){
    int src_pos, dest_pos;
    *err=0;
    adjNode *destination= NULL;

    src_pos= findArrayPosition(cities_graph, luogo_partenza);
    dest_pos= findArrayPosition(cities_graph, luogo_arrivo);

    if(src_pos==-1 || dest_pos==-1){
        printf(RED"\nerror"RESET": le citta' inserite non risultano registrate!\n");
        *err=1;
        return;
    }

    destination= getDestination(cities_graph, src_pos, dest_pos);

    if(destination){
        if(!getFlight(destination->flights_list, prezzo, data_partenza, data_arrivo, luogo_partenza, luogo_arrivo)) {

            destination->flights_list = insertFlight(destination->flights_list, prezzo, data_partenza, data_arrivo, luogo_partenza, luogo_arrivo);
            addFlightFile(destination->flights_list);
        } else
            *err=1;
    } else {
        *err=1;
        printf(RED"\nerror"RESET": si e' tentanto di inserire un volo su una tratta non esistente\n");
    }
}

void addFlightFile(volo *fly){
    FILE *fPtr;
    char temp1[MAXNAME], temp2[MAXNAME];

    fflush(stdin);
    fPtr = fopen("flights_list.txt", "a");
    if (fPtr == NULL) {
        printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
        exit(1);
    }

    strcpy(temp1, fly->luogo_partenza);
    strcpy(temp2, fly->luogo_arrivo);
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

    fprintf(fPtr, "\n%d %d %d\t%d %d %d\t%d %d\t%d %d\t%s\t%s\t%.2f", fly->data_partenza->tm_mday, fly->data_partenza->tm_mon, fly->data_partenza->tm_year,
            fly->data_arrivo->tm_mday, fly->data_arrivo->tm_mon, fly->data_arrivo->tm_year, fly->data_partenza->tm_hour, fly->data_partenza->tm_min,
            fly->data_arrivo->tm_hour, fly->data_arrivo->tm_min, temp1, temp2, fly->prezzo);
    fclose(fPtr);
}

void printFlightsRoute(tratta *cities_graph, char *src, char *dest){
    if(cities_graph){
        int src_pos= findArrayPosition(cities_graph, src);
        int dest_pos= findArrayPosition(cities_graph, dest);

        adjNode *destination= findDestination(cities_graph->vertice[src_pos]->destinations, dest_pos);

        if(destination){
            printf(GREEN"\tVOLI DISPONIBILI PER LA TRATTA: %s - %s\n"RESET, src, dest);
            printf("--------------------------------------------------------------------\n");
            printf(YELLOW"DATA PARTENZA\tORARIO\tDATA ARRIVO\tORARIO\tPREZZO(euro)\n\n"RESET);

            printFlightsList(destination->flights_list);
        }else{
            printf(RED"\nerror"RESET": la tratta %s - %s non risulta registrata\n", src, dest);
        }
    }
}

volo *getFlight(volo *flight_list, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo){
    if(flight_list){
        if(equalsDate(flight_list->data_partenza, data_partenza)
           && equalsTime(flight_list->data_partenza, data_partenza) && equalsDate(flight_list->data_arrivo, data_arrivo)
           && equalsTime(flight_list->data_arrivo, data_arrivo) && strcmp(flight_list->luogo_partenza, luogo_partenza)==0
           && strcmp(flight_list->luogo_arrivo, luogo_arrivo)==0){

            return flight_list;
        }
        return getFlight(flight_list->next, prezzo, data_partenza, data_arrivo, luogo_partenza, luogo_arrivo);
    }
    return NULL;
}

volo *deleteFlight(volo *flights_list, volo *fly){
    if(flights_list){
        if(flights_list == fly){
            //trovato il volo da eliminare
            volo *tmp= flights_list->next;
            free(flights_list->data_partenza);
            free(flights_list->data_arrivo);
            return tmp;
        }
        flights_list->next= deleteFlight(flights_list->next, fly);
    }
    return flights_list;
}

void removeFlight(tratta *cities_graph, volo *fly, int *err){
    if(cities_graph){
        if(fly) {
            *err = 0;
            int src_pos = findArrayPosition(cities_graph, fly->luogo_partenza);
            int dest_pos = findArrayPosition(cities_graph, fly->luogo_arrivo);

            if (src_pos == -1 || dest_pos == -1) {
                printf(RED"\nerror"RESET": si sta tentando di rimuovere un volo non registrato!");
                *err = 1;
                return;
            }
            adjNode *destination = getDestination(cities_graph, src_pos, dest_pos);
            if (destination) {
                removeFlightFile(fly);
                destination->flights_list = deleteFlight(destination->flights_list, fly);
            } else {
                *err = 1;
            }
        }else{
            *err=1;
        }
    }else{
        *err=1;
    }
}

void removeFlightFile(volo *fly){
    if(fly){
        FILE *fPtr, *fTemp;
        char temp1[MAXNAME], temp2[MAXNAME];
        char src[MAXNAME], dest[MAXNAME];
        float prezzo;

        fflush(stdin);
        fPtr = fopen("flights_list.txt", "r");
        fTemp = fopen("replace.tmp", "w");
        if (fPtr == NULL || fTemp == NULL) {
            printf(RED"\nerror"RESET": si e' riscontrato un errore nell'apertura del file\n");
            exit(1);
        }

        while (!feof(fPtr)) {
            struct tm *data_partenza= malloc(sizeof(struct tm));
            struct tm *data_arrivo= malloc(sizeof(struct tm));
            fscanf(fPtr, "%d %d %d\t%d %d %d\t%d %d\t%d %d\t%s\t%s\t%f", &data_partenza->tm_mday, &data_partenza->tm_mon, &data_partenza->tm_year,
                   &data_arrivo->tm_mday, &data_arrivo->tm_mon, &data_arrivo->tm_year, &data_partenza->tm_hour, &data_partenza->tm_min,
                   &data_arrivo->tm_hour, &data_arrivo->tm_min, src, dest, &prezzo);

            strcpy(temp1, src);
            strcpy(temp2, dest);
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

            if(!(equalsDate(fly->data_partenza, data_partenza)
                 && equalsTime(fly->data_partenza, data_partenza) && equalsDate(fly->data_arrivo, data_arrivo)
                 && equalsTime(fly->data_arrivo, data_arrivo) && strcmp(fly->luogo_partenza, temp1)==0
                 && strcmp(fly->luogo_arrivo, temp2)==0 && fly->prezzo==prezzo)){

                fprintf(fTemp, "%d %d %d\t%d %d %d\t%d %d\t%d %d\t%s\t%s\t%.2f\n", data_partenza->tm_mday, data_partenza->tm_mon, data_partenza->tm_year,
                        data_arrivo->tm_mday, data_arrivo->tm_mon, data_arrivo->tm_year, data_partenza->tm_hour, data_partenza->tm_min,
                        data_arrivo->tm_hour, data_arrivo->tm_min, src, dest, prezzo);
            }


        }

        int sz= ftell(fTemp);
        fseek(fTemp, sz, SEEK_SET);
        ftruncate(fileno(fTemp), sz-2);

        fclose(fPtr);
        fclose(fTemp);

        //elimino la vecchia versione del file e rinomino la nuova
        remove("flights_list.txt");
        rename("replace.tmp", "flights_list.txt");
    }
}

path *shortestDistancePath(tratta *cities_graph, char *src, char *dest) {
    if(cities_graph){
        int src_pos= findArrayPosition(cities_graph, src);
        int dest_pos= findArrayPosition(cities_graph, dest);
        if(src_pos != -1 && dest_pos != -1){
            int curr_vertex;
            path *path_list= NULL;
            path *shortest_path=NULL;
            path *temp=NULL;
            path_list= insertNode(path_list, src_pos, 0, -1);
            while(path_list){
                curr_vertex=path_list->key;
                //se nell' inserirli mi accorgo che già è presente non faccio altro che rilassare...
                path_list= scanAdjNodes(cities_graph->vertice[curr_vertex]->destinations, path_list, shortest_path, curr_vertex);
                //per farlo uso getnremove da path_list, modifico i campi next e prev in modo da costruire una nuova
                //struttura puntata da shortest path
                temp=getNRemove(&path_list,curr_vertex);
                shortest_path=insertHead(shortest_path,temp->key,temp->distance,temp->prev_vertex);
            }
            temp=getNode(shortest_path, dest_pos);
            if(temp){
                //ho raggiunto la dest
                int *array_keys=(int*)malloc(sizeof(int));
                int i=0;
                while(shortest_path){
                    array_keys[i++]=temp->key;
                    if(temp->key==src_pos)
                        break;
                    temp=getNode(shortest_path,temp->prev_vertex);
                    if(!temp)
                        break;
                    array_keys=realloc(array_keys,sizeof(int)*(i+1));
                }
                //scansioniamo la shortest path e ogni qual volta incontro un nodo la cui chiave non è presente in array_keys, lo elimino
                path *clean_shortest_path=NULL;
                for(int j=0;j<i; j++){
                    temp=getNRemove(&shortest_path,array_keys[j]);
                    clean_shortest_path=insertHead(clean_shortest_path,temp->key, temp->distance, temp->prev_vertex);
                }
                //dealloca memoria inutile
                shortest_path= destroyPathList(shortest_path);
                free(array_keys);
                return clean_shortest_path;
            }else{
                //non è possibile raggiungere dest partendo da src
                return NULL;
            }
        }else{
            printf(RED"\nerror"RESET": le citta' inserite non risultano registrate\n");
            return NULL;
        }
    }else{
        return NULL;
    }
}

path *scanAdjNodes(adjNode *adj_list, path *path_list, path *shortest_path, int src_vertex) {
    int new_distance;
    while(adj_list){
        path *newprev= getNode(path_list, src_vertex);
        path *curr=getNode(path_list,adj_list->key);
        new_distance= newprev->distance+adj_list->distanza;
        if(!getNode(shortest_path,adj_list->key)) {
            if (curr) {
                //il nodo già è presente in path_list
                relax(&path_list, &path_list, adj_list->key, new_distance, src_vertex);
            } else {
                //il nodo non è presente in path list e quindi lo devo inserire
                path_list = insertNode(path_list, adj_list->key, new_distance, src_vertex);
            }
        }
        adj_list=adj_list->next;
    }
    return path_list;
}

path *lowestPricePath(tratta *cities_graph, char *src, char *dest) {
    if(cities_graph){
        int src_pos= findArrayPosition(cities_graph, src);
        int dest_pos= findArrayPosition(cities_graph, dest);
        if(src_pos != -1 && dest_pos != -1){
            int curr_vertex;
            path *path_list= NULL;
            path *shortest_path=NULL;
            path *temp=NULL;
            path_list= insertNode(path_list, src_pos, 0, -1);
            while(path_list){
                curr_vertex=path_list->key;
                path_list= scanAdjNodesPrice(cities_graph->vertice[curr_vertex]->destinations, path_list, shortest_path, curr_vertex);
                temp=getNRemove(&path_list,curr_vertex);
                shortest_path=insertHead(shortest_path,temp->key,temp->distance,temp->prev_vertex);
            }
            temp=getNode(shortest_path, dest_pos);
            if(temp){
                int *array_keys=(int*)malloc(sizeof(int));
                int i=0;
                while(shortest_path){
                    array_keys[i++]=temp->key;
                    if(temp->key==src_pos)
                        break;
                    temp=getNode(shortest_path,temp->prev_vertex);
                    if(!temp)
                        break;
                    array_keys=realloc(array_keys,sizeof(int)*(i+1));
                }
                path *clean_shortest_path=NULL;
                for(int j=0;j<i; j++){
                    temp=getNRemove(&shortest_path,array_keys[j]);
                    clean_shortest_path=insertHead(clean_shortest_path,temp->key, temp->distance, temp->prev_vertex);
                }
                shortest_path= destroyPathList(shortest_path);

                free(array_keys);
                return clean_shortest_path;
            }else{
                return NULL;
            }
        }else{
            printf(RED"\nerror"RESET": le citta' inserite non risultano registrate\n");
            return NULL;
        }
    }else{
        return NULL;
    }
}

path *scanAdjNodesPrice(adjNode *adj_list, path *path_list, path *shortest_path, int src_vertex) {
    int new_distance;
    float min_prezzo=FLT_MAX;
    volo *cheapest_flight=NULL;
    while(adj_list){
        min_prezzo=FLT_MAX;
        cheapest_flight=NULL;
        path *newprev= getNode(path_list, src_vertex);
        path *curr=getNode(path_list,adj_list->key);
        lowestPriceFlight(adj_list->flights_list, &min_prezzo, &cheapest_flight);
        new_distance= newprev->distance+ FLOAT_TO_INT(min_prezzo);
        if(!getNode(shortest_path,adj_list->key)) {
            if (curr) {
                //il nodo già è presente in path_list
                relax(&path_list, &path_list, adj_list->key, new_distance, src_vertex);
            } else {
                //il nodo non è presente in path list e quindi lo devo inserire
                path_list = insertNode(path_list, adj_list->key, new_distance, src_vertex);
            }
        }
        adj_list=adj_list->next;
    }
    return path_list;
}

paths *possiblePaths(tratta *cities_graph, char *src, char *dest){
    if(cities_graph) {
        int src_pos = findArrayPosition(cities_graph, src);
        int dest_pos = findArrayPosition(cities_graph, dest);

        if (src_pos != -1 && dest_pos != -1) {
            int *visited = calloc(cities_graph->num_vertices, sizeof(int));
            paths *paths_list= NULL;
            path *path_list = NULL;
            findPossiblePath(cities_graph, visited, src_pos, dest_pos, path_list, &paths_list);
            return paths_list;
        } else {
            printf(RED"\nerror"RESET": le citta' inserite non risultano registrate\n");
        }
    }
    return NULL;
}

//COLORI---> 0:BIANCO; 1:GRIGIO; 2:NERO; 3:VERDE
void findPossiblePath(tratta *cities_graph, int* visited,int src_pos, int dest_pos, path *path_list, paths **paths_list) {
    int curr, found=0;
    visited[src_pos]= 1;
    path_list= insertHead(path_list, src_pos, 0, -1);
    adjNode *adj_list= cities_graph->vertice[src_pos]->destinations;
    adjNode *temp= adj_list;

    if(src_pos != dest_pos) {
        while (temp) {
            curr = temp->key;
            if (visited[curr] == 0 || visited[curr] == 3) {
                if (visited[curr] == 0)
                    visited[curr] = 1;

                findPossiblePath(cities_graph, visited, curr, dest_pos, path_list, paths_list);
            }
            temp = temp->next;
        }
        temp = adj_list;

        while (temp && !found) {
            if (visited[temp->key] == 3)
                found = 1;
            temp = temp->next;
        }
    }

    if(src_pos == dest_pos || found == 1){
        visited[src_pos]= 3;
        if(src_pos == dest_pos) {
            path *path_temp= path_list;
            path *new_path= NULL;
            while(path_temp){
                new_path= insertHead(new_path, path_temp->key, path_temp->distance, path_temp->prev_vertex);
                path_temp= path_temp->next;
            }
            *paths_list= addNewPath(*paths_list, new_path);
        }
    }else{
        visited[src_pos]= 2;
    }
    path_list= removeNode(path_list, src_pos);
}

flight_path *makeFlightSolutions(tratta *cities_graph, paths *paths_collection, struct tm *fascia_oraria) {
    flight_path *flight_solutions=NULL;
    while(paths_collection){
        path *current_path=paths_collection->list;
        flight_solutions= makeFlightsPath(cities_graph, flight_solutions, current_path, fascia_oraria); //la funzione popola flights_path con i voli che scelgo sul percorso current_path
        paths_collection=paths_collection->next;
    }
    return flight_solutions;
}

flight_path *makeFlightsPath(tratta *cities_graph, flight_path *flights_path, path *path_list, struct tm *fascia_oraria) {
    int dest_pos=-1, src_pos=-1;
    int err=0;
    time_t time_fascia_oraria, time_partenza, time_arrivo;
    fascia_oraria->tm_year-=1900;
    fascia_oraria->tm_mon--;
    time_fascia_oraria=mktime(fascia_oraria);
    fascia_oraria->tm_year+=1900;
    fascia_oraria->tm_mon++;
    //citta' di partenza
    path *curr=path_list;
    if(curr){
        src_pos=curr->key;
        curr=curr->next;
        if(curr)
            dest_pos=curr->key;
    }
    volo *start_flights_list= getFlightList(cities_graph, src_pos,dest_pos);
    //devo fare un nodo flights_path per ogni nodo di start_flights
    while(start_flights_list){
        err=0;
        volo *new_solution=NULL;
        start_flights_list->data_partenza->tm_mon--;
        start_flights_list->data_partenza->tm_year-=1900;
        start_flights_list->data_arrivo->tm_mon--;
        start_flights_list->data_arrivo->tm_year-=1900;
        time_partenza=mktime(start_flights_list->data_partenza);
        time_arrivo=mktime(start_flights_list->data_arrivo);
        start_flights_list->data_partenza->tm_mon++;
        start_flights_list->data_partenza->tm_year+=1900;
        start_flights_list->data_arrivo->tm_mon++;
        start_flights_list->data_arrivo->tm_year+=1900;
        if(difftime(time_fascia_oraria,time_partenza)<0 && fascia_oraria->tm_mday==start_flights_list->data_partenza->tm_mday && fascia_oraria->tm_mon==start_flights_list->data_partenza->tm_mon && fascia_oraria->tm_year==start_flights_list->data_partenza->tm_year){
            new_solution=insertFlight(new_solution,start_flights_list->prezzo, start_flights_list->data_partenza, start_flights_list->data_arrivo,start_flights_list->luogo_partenza,start_flights_list->luogo_arrivo);
            makeNewFlightSolution(cities_graph, &new_solution, start_flights_list->data_arrivo, curr, curr->next, &err);
        }
        if(err==0 && new_solution) {
            flights_path = addNewFlightPath(flights_path, new_solution);
        }
        start_flights_list=start_flights_list->next;
    }
    return flights_path;
}



void makeNewFlightSolution(tratta *cities_graph, volo **new_solution, struct tm *data_arrivo, path *new_source, path *new_dest, int *err) {
    //in ogni ciclo inserisco in new_solution il volo più vicino a data_arrivo da new_source a new_dest
    if(new_dest){
        volo *preferred_flight=NULL;
        volo *flights_list=getFlightList(cities_graph, new_source->key, new_dest->key);
        getExchangeFly(flights_list, data_arrivo, NULL, &preferred_flight);
        if(preferred_flight==NULL){
            *err=1;
        }else {
            *new_solution = enqueueFlight(*new_solution, preferred_flight->prezzo, preferred_flight->data_partenza, preferred_flight->data_arrivo, preferred_flight->luogo_partenza,preferred_flight->luogo_arrivo);
            makeNewFlightSolution(cities_graph, new_solution, preferred_flight->data_arrivo, new_dest, new_dest->next, err);
        }
    }
}

volo *getFlightList(tratta *cities_graph, int src_pos, int dest_pos) {
    if(cities_graph){
        adjNode *adj_list=cities_graph->vertice[src_pos]->destinations;
        while(adj_list){
            if(adj_list->key==dest_pos){
                return adj_list->flights_list;
            }
            adj_list= adj_list->next;
        }
    }
    return NULL;
}

void DFSSearch(tratta *cities_graph, int *visited, int src_pos) {
    adjNode *adj_list=cities_graph->vertice[src_pos]->destinations;
    adjNode *temp = adj_list;
    visited[src_pos] = 1;

    while (temp) {
        int connectedVertex = temp->key;

        //se bianco
        if (visited[connectedVertex] == 0)
            DFSSearch(cities_graph, visited, connectedVertex);

        temp = temp->next;
    }
    visited[src_pos]= 2;
}

int *reachableNodes(tratta *cities_graph, char *src) {
    if(cities_graph) {
        int src_pos = findArrayPosition(cities_graph, src);

        if (src_pos != -1) {
            int *visited = calloc(cities_graph->num_vertices, sizeof(int));
            DFSSearch(cities_graph, visited, src_pos);
            visited[src_pos]=0;
            return visited;
        } else {
            printf(RED"\nerror"RESET": le citta' inserite non risultano registrate\n");
        }
    }
    return NULL;
}

int *findNumBuyers(tratta *cities_graph, int *reachable_nodes) {
    int *num_buyers=calloc(cities_graph->num_vertices, sizeof(int));

    for(int i=0;i<cities_graph->num_vertices;i++){

        increaseBuyers(cities_graph->vertice[i]->destinations, reachable_nodes, num_buyers, cities_graph->num_vertices);
    }
    return num_buyers;
}

void increaseBuyers(adjNode *adj_list, int *reachable_nodes, int *num_buyers, int size) {
    if(adj_list){
        if(checkOccurrence(reachable_nodes,size,adj_list->key)){
            int num_acquirenti=getNumBuyers(adj_list->flights_list);
            num_buyers[adj_list->key]+=num_acquirenti;
        }
        increaseBuyers(adj_list->next,reachable_nodes,num_buyers,size);
    }
}

float *findLowestPrice(tratta *cities_graph, char *src, int *reachable_nodes) {
    int src_pos=findArrayPosition(cities_graph, src);
    float *lowest_prices=malloc(cities_graph->num_vertices*sizeof(float));
    for(int i=0;i<cities_graph->num_vertices;i++)
        lowest_prices[i]=FLT_MAX;

    paths *paths_list=NULL;
    for(int i=0; i<cities_graph->num_vertices;i++) {
        if (reachable_nodes[i] == 2) {
            char *dest = findNameVertex(cities_graph, i);
            path *cheaper_path = lowestPricePath(cities_graph, src,dest);
            if(cheaper_path) {
                paths_list = addNewPath(paths_list, cheaper_path);
                cheaper_path = NULL;
            }else{
                printf(RED"\nerror"RESET": impossibile trovare il path piu' economico\n");
            }
        }
    }
    paths *temp_paths=paths_list;
    while(temp_paths){
        path *temp_path=temp_paths->list;
        while(temp_path->next)
            temp_path=temp_path->next;
        lowest_prices[temp_path->key]=(float)temp_path->distance;

        temp_paths=temp_paths->next;
    }
    paths_list=destroyPathsCollection(paths_list);
    return lowest_prices;
}

int findMostPopularDestination(tratta *cities_graph, int *reachable_nodes) {
    int *buyers_nodes=findNumBuyers(cities_graph, reachable_nodes);
    int max=0, max_num_buyers=0;
    for(int i=0;i<cities_graph->num_vertices;i++){
        if(buyers_nodes[i]>max_num_buyers){
            max_num_buyers=buyers_nodes[i];
            max=i;
        }
    }
    return max;
}

int findCheapestDestination(tratta *cities_graph, int *reachable_nodes, char *src){
    float *lowest_prices=findLowestPrice(cities_graph, src, reachable_nodes);

    int min=0;
    float min_price=FLT_MAX;
    for(int i=0;i<cities_graph->num_vertices;i++){
        if(lowest_prices[i]<min_price){
            min_price=lowest_prices[i];
            min=i;
        }
    }
    return min;
}

biglietti *updateListaBiglietti(tratta *cities_graph, cliente **my_client, biglietti *ticket_list){
    if(ticket_list){
        ticket_list->next= updateListaBiglietti(cities_graph, my_client, ticket_list->next);
        char src[MAXNAME],
                dest[MAXNAME];
        strcpy(src, ticket_list->fly->luogo_partenza);
        strcpy(dest,ticket_list->fly->luogo_arrivo);
        int src_pos=findArrayPosition(cities_graph,src),
                dest_pos=findArrayPosition(cities_graph,dest);
        volo *flight_list=getFlightList(cities_graph,src_pos,dest_pos);
        if(!getFlight(flight_list,ticket_list->fly->prezzo,ticket_list->fly->data_partenza,ticket_list->fly->data_arrivo,ticket_list->fly->luogo_partenza,ticket_list->fly->luogo_arrivo)){
            time_t time_data_partenza, adesso;
            time(&adesso);
            ticket_list->fly->data_partenza->tm_year-=1900;
            ticket_list->fly->data_partenza->tm_mon--;
            time_data_partenza=mktime(ticket_list->fly->data_partenza);
            ticket_list->fly->data_partenza->tm_year-=1900;
            ticket_list->fly->data_partenza->tm_mon--;
            if(difftime(adesso,time_data_partenza)<0){
                //devo rimborsare poiche' il volo ancora non e' avvenuto
                int punti_rimborso=FLOAT_TO_INT(ticket_list->fly->prezzo)*2;
                (*my_client)->punti_sconto+=punti_rimborso;
            }
            biglietti *temp= ticket_list->next;
            free(ticket_list->data_acquisto);
            free(ticket_list);
            return temp;
        }
    }
    return ticket_list;
}

flight_path *findFlightSolutions(tratta *cities_graph, paths *paths_collection) {
    flight_path *flight_solutions=NULL;
    while(paths_collection){
        path *current_path=paths_collection->list;
        flight_solutions= findFlightsPath(cities_graph, flight_solutions, current_path);
        paths_collection=paths_collection->next;
    }
    return flight_solutions;
}

flight_path *findFlightsPath(tratta *cities_graph, flight_path *flights_path, path *path_list) {
    int dest_pos=-1, src_pos=-1;
    int err=0;
    time_t time_fascia_oraria, time_partenza, time_arrivo;

    //citta' di partenza
    path *curr=path_list;
    if(curr){
        src_pos=curr->key;
        curr=curr->next;
        if(curr)
            dest_pos=curr->key;
    }
    volo *start_flights_list= getFlightList(cities_graph, src_pos,dest_pos);
    while(start_flights_list){
        err=0;
        volo *new_solution=NULL;

        new_solution=insertFlight(new_solution,start_flights_list->prezzo, start_flights_list->data_partenza, start_flights_list->data_arrivo,start_flights_list->luogo_partenza,start_flights_list->luogo_arrivo);
        makeNewFlightSolution(cities_graph, &new_solution, start_flights_list->data_arrivo, curr, curr->next, &err);
        if(err==0 && new_solution) {
            flights_path = addNewFlightPath(flights_path, new_solution);
        }
        start_flights_list=start_flights_list->next;
    }
    return flights_path;
}

void printPathCities(tratta *cities_graph, path *path_list){
    if(cities_graph){
        if(path_list){
            if(path_list->next)
                printf(GREEN"%s "RESET"--> ", findNameVertex(cities_graph, path_list->key));
            else
                printf(GREEN"%s\n"RESET, findNameVertex(cities_graph, path_list->key));
            printPathCities(cities_graph,path_list->next);
        }
    }
}