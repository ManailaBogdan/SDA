//Manaila Bogdan 313CB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

typedef int (*TFHash)(void*);
//Structura lista generica simplu inlantuita
typedef struct celulaL
{
	void * info;
	struct  celulaL *urm;
	
} CelulaL, *TLG;

//Structura tabela Hash
typedef struct
{
    size_t M;
    TFHash fh;
    TLG *v;

} TH;

//Structura element cuvant
typedef struct {

	int nr_aparitii;
	char *cuvant;

}Cuvant;

//Functie de alocare si adaugare in lista generica
void Alocare(TLG* L, void* el)
{
	TLG aux = malloc(sizeof(CelulaL));
	if(!aux)
		printf("ER Alocare\n");
	
	aux->info = el;
	aux->urm = *L;
	*L = aux;
	
}

/*Functie de alocare spatiu tabela Hash
  si initializare M si fh*/
  
TH* InitializareTabelaHas(size_t M, TFHash fh){


	TH* h = (TH *) malloc(sizeof(TH));

    if (!h) {
        printf("ER InitializareTabelaHas1\n");
        return NULL;
    }

    h->v = (TLG *) calloc(M, sizeof(TLG));

	

    if(!h->v) {
        printf("ER InitializareTabelaHas2\n");
        free(h);
       
        return NULL;
    }

    h->M = M;
    h->fh = fh;

	

    return h;

}

//Functie ce calculeaza pozitia in v a unui cuvant
int codHash(void *el){

	char *t = (char*) el;

	if('A' <= *t && *t <= 'Z')
		return *t - 'A'; 
	if('a' <= *t && *t <= 'z')
		return *t - 'a';

    return -1;

}

//Functie ce incrementeaza numarul de aparitii al unui cuvant
void Incrementare(TH* h, char* text, int k){

    TLG L = h->v[k];
    int lg = strlen(text);
    for(; L != NULL; L = L->urm){
        if(strlen(((Cuvant*)((TLG)L->info)->info)->cuvant) == lg){

            TLG p = (TLG)(L->info);
            for( ; p != NULL; p = p->urm)
                if(strcmp(((Cuvant*)p->info)->cuvant, text) == 0){
                    {((Cuvant*)p->info)->nr_aparitii ++;
                    }
            }
        }
    }

 
}

//Functie ce verifica daca un cuvant exista deja in tabela hash
int CautareCuv(TH* h, char* text, int k){
    
    TLG L = h->v[k];
    int lg = strlen(text);
    for(; L != NULL; L = L->urm){
        if(strlen(((Cuvant*)((TLG)L->info)->info)->cuvant) == lg){

            TLG p = (TLG)(L->info);
            for( ; p != NULL; p = p->urm)
                if(strcmp(((Cuvant*)p->info)->cuvant, text) == 0)
                return 1;
        }
    }

    return 0;

}

//Functie ce verifica daca exista o sublista in tabela hash
int CauatreSubLista(TH* h, char *text, int k){

    
   
    TLG L = h->v[k];
    int lg = strlen(text);
    if(!L)
    return 0;

    for(; L != NULL; L = L->urm){

         TLG p = (TLG)(L->info);
        
     
        if(strlen(((Cuvant*)p->info)->cuvant) == lg){
            
            return 1;}
    }

    return 0;
}

//Functie ce adauga un cuvant intr-o sublista existenta
void AdCuv(TH* h, char* text, int k){


    TLG L = h->v[k];
    int lg = strlen(text);

    Cuvant *cuv = malloc(sizeof(Cuvant));
    cuv->cuvant = malloc(10000);
    
    if(!cuv)
        printf("ERR AdCuv\n");

    strcpy(cuv->cuvant, text);
    cuv->nr_aparitii = 1;

    for(; L != NULL; L = L->urm){

        if(strlen(((Cuvant*)((TLG)L->info)->info)->cuvant) == lg){
            TLG aux = malloc(sizeof(CelulaL));
            
	        if(!aux)
		    printf("ER Alocare\n");
	
	        aux->info = (void *)cuv;
	        aux->urm = L->info;
	        L->info = aux;
            
            
        }
            
    }
    


}

/*Functie ce creeaza o sublista, adauga un cuvant in ea 
  adauga sublista in v[k]*/
 
void AdSublista(TH* h, char* text, int k){
    TLG S;
    S = NULL;
    Cuvant *cuv = malloc(sizeof(Cuvant));
    cuv->cuvant = malloc(10000);

    if(!cuv)
        printf("ERR AdSublista\n");

    strcpy(cuv->cuvant, text);
    cuv->nr_aparitii = 1;

    Alocare(&S, (void *)cuv);

    Alocare(&h->v[k], (void *)S);
    

}

//Functie ce sorteaza elementele din tabela Hash
void Sortare(TH* h){

    int i;
    for(i = 0; i < h->M; i++){

        TLG L = h->v[i], p;

        if(L){

            for( ; L != NULL; L = L->urm){
                for(p = L->urm; p != NULL; p = p->urm){
                int n = strlen(((Cuvant*)((TLG)L->info)->info)->cuvant);
                int m = strlen(((Cuvant*)((TLG)p->info)->info)->cuvant);
               
                if(n > m){
                    TLG aux;
                    
                    aux = p->info;
                    p->info = L->info;
                    L->info = aux;
                  

                }

                }
            }

        }
        
    }



}

//Functie ce sorteaza elementele din subliste
void SortareSubliste(TH* h){

    int i;
    Cuvant aux;
    for(i = 0; i < h->M; i++){

        TLG L = h->v[i];

        if(L){

            for( ; L != NULL; L = L->urm){
                
                TLG p = (TLG)(L->info), q;
                 for( ; p != NULL; p = p->urm){
                     for(q = p->urm; q != NULL; q = q->urm)
                        
                        if(((Cuvant*)p->info)->nr_aparitii < ((Cuvant*)q->info)->nr_aparitii){

                           
                            aux.cuvant = malloc(10000);
                            strcpy(aux.cuvant, ((Cuvant*)p->info)->cuvant);
                            aux.nr_aparitii = ((Cuvant*)p->info)->nr_aparitii;

                            strcpy(((Cuvant*)p->info)->cuvant, ((Cuvant*)q->info)->cuvant);
                            ((Cuvant*)p->info)->nr_aparitii = ((Cuvant*)q->info)->nr_aparitii;

                            strcpy(((Cuvant*)q->info)->cuvant, aux.cuvant);
                            ((Cuvant*)q->info)->nr_aparitii = aux.nr_aparitii;
                            free(aux.cuvant);
                        }
                        else
                            if(((Cuvant*)p->info)->nr_aparitii == ((Cuvant*)q->info)->nr_aparitii)
                            if(strcmp(((Cuvant*)p->info)->cuvant, ((Cuvant*)q->info)->cuvant) > 0){
                               
                            aux.cuvant = malloc(10000);
                            strcpy(aux.cuvant, ((Cuvant*)p->info)->cuvant);
                            aux.nr_aparitii = ((Cuvant*)p->info)->nr_aparitii;

                            strcpy(((Cuvant*)p->info)->cuvant, ((Cuvant*)q->info)->cuvant);
                            ((Cuvant*)p->info)->nr_aparitii = ((Cuvant*)q->info)->nr_aparitii;

                            strcpy(((Cuvant*)q->info)->cuvant, aux.cuvant);
                            ((Cuvant*)q->info)->nr_aparitii = aux.nr_aparitii;
                            free(aux.cuvant);

                            }

                        
                 }
                
            }

        }
        
    }
}

/*Functie ce introduce un cuvant in tabela Hash,
  dupa care sorteaza tabela*/
void insert(TH* h, char* text){

    int k = codHash(text);
    
    if (k == -1)
        return;

    if(CauatreSubLista(h, text, k)){
        if(CautareCuv(h, text, k))
            Incrementare(h, text, k);

        else
            AdCuv(h, text, k);

    }

    else
        AdSublista(h, text, k);

    
     SortareSubliste(h);
    Sortare(h);
   

}

//Functie ce afisaza tabela Hash in intregime
void print1(TH* h){
    int i;
    for(i = 0; i < h->M; i++){

        TLG L = h->v[i];

        if(L){

            printf("pos %d: ", i);

            for( ; L != NULL; L = L->urm){

                TLG p = (TLG)(L->info);

                int n = strlen(((Cuvant*)p->info)->cuvant);
                printf("(%d:", n);

                for( ; p != NULL; p = p->urm){
                    
                    printf("%s/%d", ((Cuvant*)p->info)->cuvant, ((Cuvant*)p->info)->nr_aparitii);
                    if(p->urm)
                        printf(", ");
                    
                }

                printf(")");
            }

            	printf("\n");
        }
        
    }

}

//Functie ce afisaza cuvintele ce incepe cu litera k si au lungime n
void print2(TH* h, int k, int n){

   
    TLG L = h->v[k];

    for(; L != NULL; L = L->urm){

        if(strlen(((Cuvant*)((TLG)L->info)->info)->cuvant) == n){

            TLG p = (TLG)(L->info);
            printf("(%d:", n);

            for( ; p != NULL; p = p->urm){
                    
                printf("%s/%d", ((Cuvant*)p->info)->cuvant, ((Cuvant*)p->info)->nr_aparitii);
                if(p->urm)
                printf(", ");
                
                    
                }
                

        printf(")");
         break;

        }

    }
   printf("\n");
}

//Functie ce afisaza cuvintele cu numar de aparitii maxim n
void print3(TH* h, int n){

    
    int i, ok = 0, e;
    for(i = 0; i < h->M; i++){

        TLG L = h->v[i];
        
        if(L){

            e = 0;

            for( ; L != NULL; L = L->urm){

                TLG p = (TLG)(L->info);
                ok = 0;

                for( ; p != NULL; p = p->urm){
                    
                    if(((Cuvant*)p->info)->nr_aparitii <= n){
                        if(!e){
                            printf("pos%d: ", i);
                            e++;
                        }
                        if(!ok){
                            printf("(%d: ", strlen(((Cuvant*)p->info)->cuvant));
                            ok++;
                        }
                        printf("%s/%d", ((Cuvant*)p->info)->cuvant, ((Cuvant*)p->info)->nr_aparitii);
                    
                    if(p->urm)
                    if(((Cuvant*)p->urm->info)->nr_aparitii <= n)
                        printf(", ");
                    }

                    
                }
                if(ok)
                printf(")");
            }
                if(e)
            	printf("\n");
        }
        
    }

}

void Dezalocare(TH** h){

    TLG aux;
  int i;
    for(i = 0; i < (*h)->M; i++){

        TLG L = (*h)->v[i];

        if(L){

            for( ; L != NULL; ){

                TLG p = (TLG)(L->info);

                for( ; p != NULL; ){
                    
                    aux = p;
                    p = p->urm;
                    free(((Cuvant*)aux->info)->cuvant);
                    free(aux->info);
                    free(aux);
                }

                aux = L;
                L = L->urm;
                free(aux);               
            }	
        }
    }
     
    free((*h)->v);
    free(*h);
    *h = NULL;


}

int main(int argc, char* argv[])
{
   
	TH *h = NULL;
    //Calcul M
	size_t M = 'z' - 'a' + 1;
   
   //Initializare tabela Hash
	h = InitializareTabelaHas(M, codHash);

    //Deschidere fisier
    FILE *f;
	char * line = NULL;
   
    size_t len = 0;

    f = fopen(argv[1], "rt");
	if (f == NULL)
		printf("ERR fisier\n");


	
	char *functie;
	char *text;


    //Determinarea si aplicarea functie cerute
	while(getline(&line, &len, f) != -1){

        
   
        
		functie = strtok(line, " .,");
        text = strtok(NULL, " .,");
        

             
        
		if(!strcmp(functie, "insert")){	
            
            while(text){
                
		        int lg = strlen(text);

		        if(text[lg - 1] == 10)
			        text[lg - 1] = 0;
                    
                if(lg > 2)
			        insert(h, text);

                text = strtok(NULL, " .,");
               
            }

		}
		if(strstr(functie, "print")){

            
			if(text == NULL)
			    print1(h);

            else{
              
                int lg = strlen(text);

		        if(text[lg - 1] == 10)
		       text[lg - 1] = 0;
               
                char *parametru1 = malloc(10000);
      
                strcpy(parametru1, text);
              
                text = strtok(NULL, " .,");
              
                
                if(text != NULL){
                    
                    if(text[lg - 1] == 10)
			            text[lg - 1] = 0;
                    if(atoi(text) > 2)
                        print2(h, codHash(parametru1), atoi(text));
                    
                }
                else
                    print3(h, atoi(parametru1));
                
               
                free(parametru1);
             
                
            }

		}




	}

    Dezalocare(&h);
    //Inchidere fisier
    fclose(f);
    //Dezalocare functie
    free(line);
	
  
   
	
    
  
  	return 0;
}
