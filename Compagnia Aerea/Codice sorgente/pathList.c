#include "pathList.h"

path *newNode(int key, int distance, int prev_vertex) {
    path *new_node= (path*)malloc(sizeof(path));
    new_node->distance= distance;
    new_node->key= key;
    new_node->prev_vertex= prev_vertex;
    new_node->prev= NULL;
    new_node->next= NULL;
    return new_node;
}

path *insertNode(path *path_list, int key, int distance, int prev_vertex){
    if(path_list==NULL){
        path_list= newNode(key, distance, prev_vertex);
    }else{
        if (distance > path_list->distance) {
            if (path_list->next) {
                path_list->next = insertNode(path_list->next, key, distance, prev_vertex);
            } else {
                //ho scorso tutta la lista e l' elemento che devo inserire è il più grande
                path *new_node = newNode(key, distance, prev_vertex);
                path_list->next = new_node;
                new_node->prev = path_list;
            }
        } else {
            path *new_node = newNode(key, distance, prev_vertex);
            new_node->next = path_list;
            new_node->prev = path_list->prev;
            if (path_list->prev)
                path_list->prev->next = new_node;

            path_list->prev = new_node;
            return new_node;
        }
    }
    return path_list;
}

path *removeNode(path *path_list, int key) {
    if(path_list){
        if(path_list->key != key){
            path_list->next= removeNode(path_list->next, key);
        }else{
            path *temp= path_list;
            path_list= temp->next;
            if(path_list)
                path_list->prev= temp->prev;
            free(temp);
        }
    }
    return path_list;
}

path *getNRemove(path **path_list, int key) {
    if(*path_list){
        if((*path_list)->key != key){
            return getNRemove(&(*path_list)->next, key);
        }else {
            path *temp = newNode(key, (*path_list)->distance, (*path_list)->prev_vertex);
            *path_list=removeNode(*path_list,key);
            /*if ((*path_list)->prev && temp)
                (*path_list)->prev->next = (*path_list)->next;

            if ((*path_list)->next && temp)
                (*path_list)->next->prev = (*path_list)->prev;
*/

            return temp;
        }
    }
    return NULL;
}

void relax(path **head, path **curr, int key, int new_distance, int new_prev_vertex) {
    if(*curr){
        if((*curr)->key != key){
            relax(head, &(*curr)->next, key, new_distance, new_prev_vertex);
        }else{
            if(new_distance < (*curr)->distance){
                if((*curr)->distance < (*curr)->prev->distance){
                    *head= removeNode(*head, key);
                    *head= insertNode(*head, key, new_distance, new_prev_vertex);
                }else{
                    (*curr)->distance= new_distance;
                    (*curr)->prev_vertex= new_prev_vertex;
                }
            }
        }
    }else{
        printf(RED"error"RESET" occurred in function relax\n");
    }
}

void printPathList(path *path_list) {
    if(path_list){
        if(path_list->prev && path_list->next){
            printf("["GRAY"%d"RESET"|%d|%d|%d|"GRAY"%d"RESET"]->", path_list->prev->key, path_list->key, path_list->distance, path_list->prev_vertex,
                   path_list->next->key);
        }else if(path_list->prev){
            printf("["GRAY"%d"RESET"|%d|%d|%d|"GRAY"NULL"RESET"]->", path_list->prev->key, path_list->key, path_list->distance, path_list->prev_vertex);
        }else if(path_list->next){
            printf("["GRAY"NULL"RESET"|%d|%d|%d|"GRAY"%d"RESET"]->", path_list->key, path_list->distance, path_list->prev_vertex, path_list->next->key);
        }else{
            printf("["GRAY"NULL"RESET"|%d|%d|%d|"GRAY"NULL"RESET"]->", path_list->key, path_list->distance, path_list->prev_vertex);
        }
        printPathList(path_list->next);
    }else{
        printf("NULL\n");
    }
}

path *getNode(path *path_list, int key) {
    if(path_list){
        if(path_list->key == key)
            return path_list;

        return getNode(path_list->next, key);
    }
    return NULL;
}

path *insertHead(path *path_list, int key, int distance, int prev_vertex) {
    path *new_node=newNode(key, distance, prev_vertex);
    if(path_list){
        new_node->next=path_list;
        path_list->prev=new_node;
    }
    return new_node;
}

path *destroyPathList(path *path_list) {
    if(path_list){
        path *temp=path_list;
        path_list=path_list->next;
        free(temp);
        temp=NULL;
        path_list=destroyPathList(path_list);
    }
    return path_list;
}

paths *destroyPathsCollection(paths *paths_list){
    if(paths_list){
        paths *temp=paths_list;
        paths_list=paths_list->next;
        temp->list=destroyPathList(temp->list);
        temp->list=NULL;
        paths_list=destroyPathsCollection(paths_list);
    }
    return paths_list;
}

paths *addNewPath(paths *paths_list, path *percorso) {
    if (percorso) {
        paths *new_path = malloc(sizeof(paths));
        new_path->list = percorso;
        new_path->next = paths_list;
        return new_path;
    }
    return paths_list;
}

void printPathsList(paths *paths_list) {
    if(paths_list){
        if(paths_list->list)
            printPathList(paths_list->list);

        printPathsList(paths_list->next);
    }else{
        printf("NULL\n");
    }
}