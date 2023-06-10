#include "flightsLists.h"

volo *insertFlight(volo *flights_list, float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo){
    volo *new_flight= newFlight(prezzo, data_partenza, data_arrivo, luogo_partenza, luogo_arrivo);
    if(flights_list)
        new_flight->next= flights_list;

    return new_flight;
}

volo *enqueueFlight(volo *flights_list, float prezzo, struct tm *data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo) {
    if(flights_list){
        flights_list->next=enqueueFlight(flights_list->next,prezzo, data_partenza,data_arrivo,luogo_partenza,luogo_arrivo);
    }else{
        return newFlight(prezzo, data_partenza,data_arrivo,luogo_partenza,luogo_arrivo);
    }
    return flights_list;
}

volo *newFlight(float prezzo, struct tm* data_partenza, struct tm *data_arrivo, char *luogo_partenza, char *luogo_arrivo){
    volo *new_flight= (volo*)malloc(sizeof(volo));
    new_flight->num_acquirenti=0;
    new_flight->prezzo= prezzo;
    new_flight->data_partenza= data_partenza;
    new_flight->data_arrivo= data_arrivo;
    strcpy(new_flight->luogo_partenza, luogo_partenza);
    strcpy(new_flight->luogo_arrivo, luogo_arrivo);
    new_flight->next= NULL;
    return new_flight;
}

int getNumBuyers(volo *flights_list){
    if(flights_list)
        return flights_list->num_acquirenti + getNumBuyers(flights_list->next);

    return 0;
}

void printFlightsList(volo *flights_list){
    if(flights_list){
        printf("\t%d/%d/%d\t%d:%d da %s    \t%d/%d/%d\t%d:%d a %s     \t%.2f\n", flights_list->data_partenza->tm_mday, flights_list->data_partenza->tm_mon,flights_list->data_partenza->tm_year, flights_list->data_partenza->tm_hour, flights_list->data_partenza->tm_min, flights_list->luogo_partenza,
               flights_list->data_arrivo->tm_mday, flights_list->data_arrivo->tm_mon, flights_list->data_arrivo->tm_year,
               flights_list->data_arrivo->tm_hour, flights_list->data_arrivo->tm_min, flights_list->luogo_arrivo, flights_list->prezzo);
        printFlightsList(flights_list->next);
    }
}

void lowestPriceFlight(volo *flights_list, float *min_prezzo, volo **cheapest_flight) {
    if(flights_list){
        double difftempo=0;
        //modifiche necessarie affinché mktime non ritorni -1
        flights_list->data_partenza->tm_year-=1900;
        flights_list->data_partenza->tm_mon--;
        flights_list->data_partenza->tm_sec=0;
        time_t adesso, partenza;
        time(&adesso);
        partenza=mktime(flights_list->data_partenza);
        difftempo=(double)difftime(partenza, adesso);
        //restauro i valori originali dei campi di data_partenza
        flights_list->data_partenza->tm_year+=1900;
        flights_list->data_partenza->tm_mon++;
        if(difftempo>0) {
            float new_min_prezzo = flights_list->prezzo;
            if (new_min_prezzo < *min_prezzo) {
                *min_prezzo = new_min_prezzo;
                *cheapest_flight = flights_list;
            }
        }
        lowestPriceFlight(flights_list->next, min_prezzo, cheapest_flight);
    }
}

flight_path *addNewFlightPath(flight_path *flight_collection, volo *new_flight_path) {
    if(new_flight_path) {
        flight_path *new_path = malloc(sizeof(flight_path));
        new_path->flight=new_flight_path;
        if(flight_collection){
            new_path->next = flight_collection;
        }else{
            new_path->next=NULL;
        }
        return new_path;
    }
    return flight_collection;
}

void printFlightsPath(flight_path *flight_collection, int i) {
    if(flight_collection){
        if(flight_collection->flight) {
            printf("\n"GREEN"[%d]. "RESET, i);
            i++;
            printFlightsList(flight_collection->flight);
        }
        printFlightsPath(flight_collection->next, i);
    }
}

void getExchangeFly(volo *flights_list, struct tm *data_arrivo, struct tm *prossima_data_partenza, volo **preferred_flight) {
    if(flights_list){
        data_arrivo->tm_year-=1900;
        data_arrivo->tm_mon--;
        flights_list->data_partenza->tm_year-=1900;
        flights_list->data_partenza->tm_mon--;
        flights_list->data_partenza->tm_isdst=-1;

        if(prossima_data_partenza) {
            prossima_data_partenza->tm_mon--;
            prossima_data_partenza->tm_year-=1900;
        }

        time_t  time_arrivo=mktime(data_arrivo),
                time_partenza=mktime(flights_list->data_partenza),
                time_prossima_data_partenza=mktime(prossima_data_partenza);

        data_arrivo->tm_year+=1900;
        data_arrivo->tm_mon++;
        flights_list->data_partenza->tm_year+=1900;
        flights_list->data_partenza->tm_mon++;

        if(prossima_data_partenza) {
            prossima_data_partenza->tm_mon++;
            prossima_data_partenza->tm_year+=1900;
        }

        if(difftime(time_arrivo,time_partenza)<0 && difftime(time_arrivo,time_partenza)>-(float)(3600*3)){
            if(difftime(time_partenza,time_prossima_data_partenza)<0 || time_prossima_data_partenza==-1){
                prossima_data_partenza=flights_list->data_partenza;
                *preferred_flight=flights_list;
            }
        }
        getExchangeFly(flights_list->next,data_arrivo, prossima_data_partenza,preferred_flight);
    }
}

int getNumTravel(flight_path *flight_collection){
    if(flight_collection)
        return 1+getNumTravel(flight_collection->next);
    return 0;
}

volo *getTravel(flight_path *flight_collection, int numero_soluzione){
    if(flight_collection){
        if(numero_soluzione==1)
            return flight_collection->flight;
        return getTravel(flight_collection->next,numero_soluzione-1);
    }
    return NULL;
}