/*-- testHash.c  */
#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>



typedef struct{

	int nr_litere;
	TLG Cuvinte;

}TL1;

typedef struct{

	int nr_aparitii;
	char *cuvant;

}TCuvant;

int codHash(void * element)
{
	TCarte * carte = (TCarte *) element;
	char * autor = carte->autor;
	return *autor - 'A';
}

int codHashPers(void * element)
{
	TPers * pers = (TPers *) element;
	char * nume = pers->nume;
	return *nume - 'A';
}

void afisareCarte(void * element)
{
	TCarte * carte = (TCarte *) element;
	printf("%s - %s - %s\n", carte->autor, carte->titlu, carte->editura);
}

void afisarePers(void * element)
{
	TPers * pers = (TPers *) element;
	printf("%s - %d\n", pers->nume, pers->varsta);
}

void input(TLG *aL, char *sir){

	TLG L = *aL;

	if(L == NULL)
		


}

TLG citesteListaCarti(char * numeFisier) {
	FILE *f;
	char * line = NULL;
	size_t len = 0;
	TLG L = NULL;

	f = fopen(numeFisier, "rt");
	if (f == NULL)
		return NULL;

	while (getline(&line, &len, f) != -1) {
		char * autor = strtok(line, ":");
		char * titlu = strtok(NULL, ":");
		char * editura = strtok(NULL, ":");

		if (editura[strlen(editura) - 1] == '\n')
			editura[strlen(editura) - 1] = '\0';

		TCarte * carte = malloc(sizeof(TCarte));
		if (carte == NULL)
			return L;

		strcpy(carte->autor, autor);
		strcpy(carte->titlu, titlu);
		strcpy(carte->editura, editura);
		Ins_IncLG(&L, (void *) carte);
    }
	fclose(f);
	return L;
}
int nr_char_int(char *s){

	int n = strlen(s), nr = 0;

	for(int i = 0; i < n; i ++)
		nr = nr * 10 + s[i] - '0'; 


	return nr;

}


TLG citesteListaPers(char * numeFisier) {
	FILE *f;
	char * line = NULL;
	size_t len = 0;
	TLG L = NULL;
	

	f = fopen(numeFisier, "rt");
	if (f == NULL)
		return NULL;

	
	while (getline(&line, &len, f) != -1) {
		

		char * nume = strtok(line, ":");
		char * varsta = strtok(NULL, ":");
		

		if (varsta[strlen(varsta) - 1] == '\n')
			varsta[strlen(varsta) - 1] = '\0';
		
		int v = nr_char_int(varsta);
		
		//printf("%s %d\n", nume, v);

		TPers * pers = malloc(sizeof(TPers));
		pers->nume = malloc(100);
		
		if (pers == NULL)
			return L;
		

		strcpy(pers->nume, nume);
		pers->varsta = v;
		Ins_IncLG(&L, (void *) pers);
    }
	fclose(f);
	return L;
}

int cmplgnume(char *n1, char *n2){

	if(strlen(n1) < strlen(n2))
		return 1;

	return 0;

}

void sortare(TLG *aL){

	TLG L, M;
	for(L = *aL; L != NULL; L = L->urm)
		for(M = L->urm; M!=NULL; M = M->urm)
			if(cmplgnume(((TPers*)L->info)->nume,((TPers*)M->info)->nume)){

					TPers aux;
					aux.nume = malloc(100);
					strcpy(aux.nume, ((TPers*)L->info)->nume);
					aux.varsta = ((TPers*)L->info)->varsta;

					strcpy(((TPers*)L->info)->nume, ((TPers*)M->info)->nume);
					((TPers*)L->info)->varsta = ((TPers*)M->info)->varsta;

					strcpy(((TPers*)M->info)->nume, aux.nume);
					((TPers*)M->info)->varsta = aux.varsta;


			}
			


}

int cmpCarte(void * e1, void * e2)
{
	TCarte * carte1 = (TCarte *) e1;
	TCarte * carte2 = (TCarte *) e2;

	if (strcmp(carte1->titlu, carte2->titlu) != 0)
		return 0;

	if (strcmp(carte1->autor, carte2->autor) != 0)
		return 0;

	if (strcmp(carte1->editura, carte2->editura) != 0)
		return 0;

	return 1;
}

int cmpPers(void * e1, void * e2)
{
	TPers * p1 = (TPers *) e1;
	TPers * p2 = (TPers *) e2;

	if (strcmp(p1->nume, p2->nume) != 0)
		return 0;

	if(p1->varsta != p2->varsta)
	return 0;
	return 1;
}

TH * GenerareHash(TLG listaCarti)
{
    TH *h = NULL;
	TLG p;
    TCarte * carte;
	int rez;

	/* calcul dimensiuni maxime pt tabela hash */
	size_t M = ('Z'-'A');

	/* initializare tabela hash */
	h = (TH *) InitTH(M, codHash);
	if(h == NULL)
		return NULL;

	for(p = listaCarti; p != NULL; p = p->urm) {
        carte = (TCarte *) malloc(sizeof(TCarte));
        if(carte == NULL)
						return h;

        memcpy(carte, p->info, sizeof(TCarte));
		rez = InsTH(h, carte, cmpCarte);
        if(!rez) {
						free(carte);
						return h;
		}
	}

	return h;
}

TH * GenerareHashPers(TLG listaCarti)
{
    TH *h = NULL;
	TLG p;
    TPers * pers;
	int rez;

	/* calcul dimensiuni maxime pt tabela hash */
	size_t M = ('Z'-'A');

	/* initializare tabela hash */
	h = (TH *) InitTH(M, codHashPers);
	if(h == NULL)
		return NULL;

	for(p = listaCarti; p != NULL; p = p->urm) {
        pers = (TPers *) malloc(sizeof(TPers));
        if(pers == NULL)
						return h;

        memcpy(pers, p->info, sizeof(TPers));
		rez = InsTH(h, pers, cmpPers);
        if(!rez) {
						free(pers);
						return h;
		}
	}

	return h;
}

int main()
{
	/* Citeste o lista de carti din fisier */
	TLG listaCarti = citesteListaCarti("carti.txt");
	if (listaCarti == NULL) {
		printf("Lista nu a putut fi generata\n");
        return 0;
	}

	printf("=========== LISTA CARTI ===========\n");
	Afisare(&listaCarti, afisareCarte);

	TH * h = NULL;
	h = GenerareHash(listaCarti);
	if (h == NULL) {
		printf("Tabela hash nu a putut fi generata\n");
        return 0;
	}

	printf("\n\n=========== TABELA HASH ===========\n");
	AfiTH(h, afisareCarte);
	

  	DistrugeLG(&listaCarti, free);
	DistrTH(&h, free);
	
	

  	return 0;
}
