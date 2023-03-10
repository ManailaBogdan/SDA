/*Manaila Bogdan - 313CB*/
#include "Cozi.h"

//Functie initializare coada
void* InitQ(){
    
    AC a = (AC)malloc(sizeof(Coada));
    
    if(!a)
    {
        printf("\nERR InitQ!!\n");
        return NULL;
    }

    a->ic = NULL;
    a->sc = NULL;

    return (void*)a;
}

//Functie introducere element in coada
void IntrQ(void *C, void *info){

    AC a = (AC)C;
    if(!a->sc)
    {
        TL2G aux = malloc(sizeof(CelulaL2));
        aux->info = info;
        aux->urm = NULL;
        a->ic = aux;
        a->sc = a->ic;
    }

    else
    {
        TL2G aux = malloc(sizeof(CelulaL2));
        aux->info = info;
        aux->urm = NULL;
        a->sc->urm = aux;
        a->sc = a->sc->urm;
    }
}

//Functie extragere element din coada
void ExtrQ(void* C, void* el){
    AC a = (AC) C;
    memcpy(el, a->ic->info, sizeof(Task));
   
    if(!a->ic->urm)
    {
        TL2G aux = a->ic;
        free(aux->info);
        free(aux);
        a->sc = NULL;
    }
    else
    {
        TL2G aux = a->ic;
        a->ic = a->ic->urm;
        free(aux->info);
        free(aux);
    }
       
}

//Functie de concatenare cozi
void ConcatQ(void* a, void* b){

    AC C1, C2;
    C1 = (AC)a;
    C2 = (AC)b;

    C1->sc->urm = C2->ic;
    C1->sc = C2->sc;
    
}

//Functie distrugere coada
void DistrQ(void **a){
    
    AC C = (AC) (*a);
    void *el = malloc(sizeof(Task));
    while(C->sc)
    {
        ExtrQ((*a), el);
    }

    free(el);
    free(*a);

}

//Functie de copiere a unei cozi
void MutaQ(void **a, void *b){

    AC C1, C2;
    C1 = (AC)*a;
    C2 = (AC)b;
    C1->ic = C2->ic;
    C1->sc = C2->sc;

}
