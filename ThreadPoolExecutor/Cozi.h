/*Manaila Bogdan - 313CB*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct task //structura unui task
{
    int id; //id-ul unui task
    int time; //timpul necesar pentru rularea completa a task-ului
    int time_ex; //timpul executat pentru un task
    int pri; //prioritatea unui task
    int Thread; //thread-ul ce ruleaza un task
}Task;

typedef struct celula2 //Celula lista generica pentru coada
{
    void *info;
    struct  celula2 *urm;
    
}CelulaL2, *TL2G;

typedef struct coada //Structura coada
{
    TL2G ic; //inceput coada
    TL2G sc; //final coada

}Coada, *AC;

void* InitQ();
void IntrQ(void*, void*);
void ExtrQ(void*, void*);
void ConcatQ(void*, void*);
void DistrQ(void**);
void MutaQ(void**, void*);

