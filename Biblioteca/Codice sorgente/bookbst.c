#include "bookbst.h"

book *initializeLibrary(void){
    book *library=NULL;

    library= addBook(library, "La coscienza di Zeno", "Italo Svevo", 1); //8
    library= addBook(library, "Il giro del mondo in 80 giorni", "Jules Verne", 2); //4
    library= addBook(library, "Ultime lettere di Jacopo Ortis", "Ugo Foscolo", 3); //12
    library= addBook(library, "Dracula", "Bram Stoker", 3); //2
    library= addBook(library, "Il ritratto di Dorian Gray", "Oscar Wilde", 2); //6
    library= addBook(library, "Assassinio sull' Orient Express", "Agatha Christie", 3); //1
    library= addBook(library, "Il fu Mattia Pascal", "Luigi Pirandello", 2); //3
    library= addBook(library, "Oliver Twist", "Charles Dickens", 2); //10
    library= addBook(library, "Ventimila leghe sotto i mari", "Jules Verne", 3); //14
    library= addBook(library, "Zanna bianca", "Jack London", 4); //15
    library= addBook(library, "Il signore delle mosche", "William Golding", 2); //7
    library= addBook(library, "Il piccolo principe", "Antoine de Saint-Exupery", 3); //5
    library= addBook(library, "Uno, nessuno e centomila", "Luigi Pirandello", 1); //13
    library= addBook(library, "Sei personaggi in cerca di autore", "Luigi Pirandello", 1); //11
    library= addBook(library, "Moby Dick", "Herman Melville", 6); //9
    return library;
}

book *addBook(book *newbook, char *title, char *author, int copies){
    if(newbook){
        if(strcmp(newbook->title,title)<=0){
            newbook->right=addBook(newbook->right, title, author, copies);
        }else{
            newbook->left=addBook(newbook->left, title, author, copies);
        }
    }else{
        newbook=newBook(title,author,copies);
    }
    return newbook;
}

book *newBook(char *title, char *author, int copies){
    book *newbook=(book*)malloc(sizeof(book));
    strcpy(newbook->title, title);
    strcpy(newbook->author, author);
    newbook->availability=copies;
    newbook->left=NULL;
    newbook->right=NULL;
    return newbook;
}

char *getMin(book *tree){
    if(tree){
        if(tree->left){
            return getMin(tree->left);
        }else{
            return tree->title;
        }
    }else
        return NULL;
}

void deleteBook(book **tree, char *title){
    if(*tree){
        if(strcmp((*tree)->title,title)==0){
            //trovato l'elemento
            book *aux=NULL;
            if(!(*tree)->left && !(*tree)->right){
                free(*tree);
                *tree=NULL;
            }else if((*tree)->left && !(*tree)->right){
                //il nodo ha il figlio sinistro
                aux= *tree;
                *tree= (*tree)->left;
                free(aux);
                aux=NULL;
            }else if(!(*tree)->left && (*tree)->right){
                //il nodo ha il figlio destro
                aux= *tree;
                *tree= (*tree)->right;
                free(aux);
                aux=NULL;
            }else if((*tree)->left && (*tree)->right){
                strcpy((*tree)->title,getMin((*tree)->right));
                deleteBook(&(*tree)->right, (*tree)->title);
            }
        }else{
            if(strcmp((*tree)->title, title)>0){
                deleteBook(&(*tree)->left, title);
            }else{
                deleteBook(&(*tree)->right, title);
            }
        }
    }
}

void destroyLibrary(book **library){
    if(*library){
        destroyLibrary(&(*library)->left);
        destroyLibrary(&(*library)->right);
        free(*library);
        *library=NULL;
    }
}

book *getBook(book *library, char *title){
    if(library){
        if(strcmp(library->title,title)<0){
            return getBook(library->right, title);
        }else if(strcmp(library->title,title)>0){
            return getBook(library->left, title);
        }else{
            return library;
        }
    }
    return NULL;
}

void preOrderStamp(book *library){
    if(library){
        printf("\t-> %s di %s [",library->title, library->author);
        if(library->availability==0){
            printf(RED);
        }else{
            printf(GREEN);
        }
        printf("%d"RESET" copie disponibili]\n",library->availability);
        preOrderStamp(library->left);
        preOrderStamp(library->right);
    }
}

void postOrderStamp(book *library){
    if(library){
        preOrderStamp(library->left);
        preOrderStamp(library->right);
        printf("\t-> %s di %s [",library->title, library->author);
        if(library->availability==0){
            printf(RED);
        }else{
            printf(GREEN);
        }
        printf("%d"RESET" copie disponibili]\n",library->availability);
    }
}

void inOrderStamp(book *library){
    if(library){
        preOrderStamp(library->left);
        printf("\t-> %s di %s [",library->title, library->author);
        if(library->availability==0){
            printf(RED);
        }else{
            printf(GREEN);
        }
        printf("%d"RESET" copie disponibili]\n", library->availability);
        preOrderStamp(library->right);
    }
}

int checkAvailability(book *library, char *title) {
    if (library) {
        if (strcmp(title, library->title) > 0) {
            return checkAvailability(library->right, title);
        } else if (strcmp(title, library->title) < 0) {
            return checkAvailability(library->left, title);;
        } else {
            if (library->availability > 0) {
                return 1;
            } else {
                return 0;
            }
        }
    }
    return -1;
}