#include <stdio.h>
#include <string.h>
/*Manaila Bogdan - 313CB*/
#include <stdlib.h>

typedef struct celula1 //Celula din lista generica pentru stiva
{
    void *info;
    struct  celula1 *urm;

}CelulaL1, *TL1G;

typedef struct stiva //Structura stiva
{
    TL1G vs; //Varful stivei

}Stiva, *AS;

void* InitS();
void Push(void* , void*);
void Pop(void*, void*);
void DistrS(void **);

