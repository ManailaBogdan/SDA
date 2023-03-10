/*Manaila Bogdan - 313CB*/
#include "Stive.h"

//Functie initializare stiva
void* InitS(){ 

    AS a = (AS)malloc(sizeof(Stiva));
    if(!a)
    {
        printf("\nERR InitS!!\n");
        return NULL;
    }

    
    a->vs = NULL;
    

    return (void*)a;
}

//Functie adaugare element in stiva
void Push(void *S, void* info){

    AS a = (AS)S;
    if(!a->vs)
    {
        TL1G aux = malloc(sizeof(CelulaL1));
        aux->info = info;
        aux->urm = NULL;
        a->vs = aux;
    }

    else
    {
        TL1G aux = malloc(sizeof(CelulaL1));
        aux->info = info;
        aux->urm = a->vs;
        a->vs = aux;
    }

}

//Functie extragere element din stiva
void Pop(void *S, void *el){

    AS a = (AS) S;

    memcpy(el, a->vs->info, sizeof(int));

    if(!a->vs->urm)
    {
        TL1G aux;
        aux = a->vs;
        free(aux->info);
        free(aux);
        a->vs = NULL;
    }
    else
    {
        TL1G aux;
        aux = a->vs;
        a->vs = a->vs->urm;
        free(aux->info);
        free(aux);
    }

}


//Functie distrugere stiva
void DistrS(void **a){

    AS S = (AS) (*a);
    void *el = malloc(sizeof(int));

    while(S->vs)
    {
        Pop((*a), el);
    }

    free(el);
    free(*a);
}