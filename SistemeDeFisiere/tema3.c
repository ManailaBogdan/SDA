//Manaila Bogdan 313CB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct  fisier //Structura pentru Arbore binar de fisiere
{
    char *nume; //nume fisier
    struct  director *parinte; //directorul parinte
    struct  fisier *st; //copilul din stanga
    struct  fisier *dr; //copilul din dreapta
    
}Fis, *AFIS;


typedef struct  director //structura pentru arbore binar de directoare
{
    char *nume; //nume director
    struct director *parinte; // directorul parinte
    AFIS fisier; // radacina arborelui binar de fisiere
    struct  director *subdir; // radacina arborelui de subdirectoare
    struct  director *st; // copilul din staga
    struct  director *dr; // copilul din dreapta

}Dir, *ADIR;

void touch(void *d, char *nume) //Functie de adaugare fisier in arbore
{

    ADIR D = (ADIR) d;
    ADIR sd = D->subdir;
    while(sd) // cautam daca exista deja un director cu acelasi nume
    {
        if(strcmp(sd->nume, nume) == 0)
        {
            printf("Directory %s already exists!\n", nume);
            return;
        }
            else if(strcmp(sd->nume, nume) < 0)
                    sd = sd->dr;
                else
                    sd = sd->st;
        }


    if(!D->fisier) // daca nu exista inca arborele de fisiere se creeaza radacina acestuia
    {
        AFIS F = malloc(sizeof(Fis));
        F->nume = malloc(51);
        strcpy(F->nume, nume);
        F->parinte = D;
        F->st = NULL;
        F->dr = NULL;

        D->fisier = F;
    }

    else //arborele de fisiere este existent
    {
        AFIS f = D->fisier;
        while(f) //cautam daca exista deja un fisier cu acelasi nume
        {
            
            if(strcmp(f->nume, nume) == 0)
            {
                printf("File %s already exists!\n", nume);
                return;
            }
            if(strcmp(f->nume, nume) < 0)
            {
                if(!f->dr) // adaugam fisierul pe pozitia potrivita
                {
                    f->dr = malloc(sizeof(Fis));
                    f->dr->nume = malloc(51);
                    strcpy(f->dr->nume, nume);
                    f->dr->parinte = D;
                    f->dr->st = NULL;
                    f->dr->dr = NULL;

                    break;
                }
                else
                    f = f->dr;
            } 
            else
            {
                if(!f->st)  // adaugam fisierul pe pozitia potrivita
                {
                    f->st = malloc(sizeof(Fis));
                    f->st->nume = malloc(51);
                    strcpy(f->st->nume, nume);
                    f->st->parinte = D;
                    f->st->st = NULL;
                    f->st->dr = NULL;
                    break;
                }
                else
                    f = f->st;
            }

            
        }  
    }
}

void mkdir(void *d, char *nume) //Functie de adaugare director
{

    ADIR D = (ADIR) d;

    if(!D->subdir) //Daca nu exista arborele de subdirectoare se creeaza radacina
    {
        ADIR SD = calloc(1, sizeof(Dir));
        SD->nume = malloc(51);
        strcpy(SD->nume, nume);
        SD->parinte = D;
        SD->st = NULL;
        SD->dr = NULL;
        D->subdir = SD;
    }

    else // arborele este deja existent
    {
       
        AFIS f = D->fisier;
        while(f) //Cautam daca exista deja un fisier cu acelasi nume
        {
            if(strcmp(f->nume, nume) == 0)
            {
                printf("File %s already exists!\n", nume);
                return;
            }
            else if(strcmp(f->nume, nume) < 0)
                    f = f->dr;
                else
                    f = f->st;
        }

        ADIR sd = D->subdir;
        while(sd) //cautam daca exista deja un director cu acelasi nume 
        {
            
            if(strcmp(sd->nume, nume) == 0)
            {
                printf("Directory %s already exists!\n", nume);
                return;
            }
            if(strcmp(sd->nume, nume) < 0)
            {
                if(!sd->dr) // adaugam directorul pe pozitia potrivita
                {
                    sd->dr = calloc(1, sizeof(Dir));
                    sd->dr->nume = malloc(51);
                    strcpy(sd->dr->nume, nume);
                    sd->dr->parinte = D;
                    sd->dr->st = NULL;
                    sd->dr->dr = NULL;
                    sd->dr->fisier = NULL;
                    sd->dr->subdir = NULL;
                    break;
                }
                else
                    sd = sd->dr;
            } 
            else
            {
                if(!sd->st) // adaugam directorul pe pozitia potrivita
                {
                    sd->st = calloc(1, sizeof(Dir));
                    sd->st->nume = malloc(51);
                    strcpy(sd->st->nume, nume);
                    sd->st->parinte = D;
                    sd->st->st = NULL;
                    sd->st->dr = NULL;
                    sd->st->fisier = NULL;
                    sd->st->subdir = NULL;
                    break;
                }
                else
                    sd = sd->st;
            }

            
        }
    }
}

void print_lex_dir(ADIR D) //Functie ce printeaza in ordine lexicografica directoarele
{
    if(!D)
        return;

    print_lex_dir(D->st);
    printf("%s ", D->nume);
    print_lex_dir(D->dr);
}

void print_lex_fis(AFIS F)  //Functie ce printeaza in ordine lexicografica directoarele
{
    if(!F)
        return;

    print_lex_fis(F->st);
    printf("%s ", F->nume);
    print_lex_fis(F->dr);
}

void ls(void *d)    //Functie ce printeaza fisierele si directoarele dintr-un director
{
    ADIR D = (ADIR) d;

    print_lex_dir(D->subdir);
    print_lex_fis(D->fisier);
    printf("\n");

}

void rm(void* d, char* nume) //Functie ce sterge un fisier
{
    ADIR D = (ADIR) d;

    AFIS F = D->fisier;
    AFIS f = NULL;

    while(F) //Cautam adresa fisierului ce trebuie sters
    {
        if(strcmp(F->nume, nume) == 0)
            break;
        f = F;
        if(strcmp(F->nume, nume) < 0)
            F = F->dr;
        else
            F = F->st;
    }

    if(!F)  //Fisierul nu exista
    {
        printf("File %s doesn't exist!\n", nume);
        return;
    }

    if(!F->st && !F->dr) //fisierul nu are copii
    {
        if(!f) //fisierul sters este radacina
           {
               free(D->fisier->nume);
               free(D->fisier);
                D->fisier = NULL;
            }
        else //fisierul sters nu este radacina
        {
            if(f->st) 
                if(strstr(f->st->nume, nume))
                {
                    free(f->st->nume);
                    free(f->st);
                    f->st = NULL;
                }
            if(f->dr)
                if(strstr(f->dr->nume, nume) && f->dr)
                {
                    free(f->dr->nume);
                    free(f->dr);
                    f->dr = NULL;
                }
        }
        return;
    }

    if(!F->st) //fisierul are doar copilul drept
    {
        AFIS aux = F->dr;
        stpcpy(F->nume, F->dr->nume);
        F->st = F->dr->st;
        F->dr = F->dr->dr;
        free(aux->nume);
        free(aux);
        return;
    }

    if(!F->dr) //fisierul are doar copilul stang
    {
        AFIS aux = F->st;
        strcpy(F->nume, F->st->nume);
        F->dr = F->st->dr;
        F->st = F->st->st;
        free(aux->nume);
        free(aux);
        return;
    }

    //fisierul are ambii copii
    AFIS aux1 = F->dr;
    AFIS aux2 = F->dr->st;

    f = F->st; 
    while(f) 
    {
        if(!f->dr)
        {
            f->dr = aux2;
            break;
        }
        f = f->dr;
    }

    //sterg fisierul
    F->dr->st = NULL;
    strcpy(F->nume, F->dr->nume);
    F->dr = F->dr->dr;

    free(aux1->nume);
    free(aux1);
}

void distr_arb_fis(AFIS F) //Functie ce dezaloca si distruge arborele de fisiere 
{
    if(!F)
        return;

    if(F->st)
        distr_arb_fis(F->st);

    if(F->dr)
        distr_arb_fis(F->dr);

    free(F->nume);
    free(F);
    F = NULL;  
}

void distr_arb_dir(ADIR D) //Functie ce dezaloca si distruge arborele de directoare
{
    if(!D)
        return;

    distr_arb_fis(D->fisier);

    if(D->st)
        distr_arb_dir(D->st);

    if(D->dr)
        distr_arb_dir(D->dr);

    if(D->subdir)
        distr_arb_dir(D->subdir);

    free(D->nume);
    free(D);
    D = NULL;  
}


void rmdir(void *d, char *nume) //Functie ce sterge un director
{
    ADIR D = (ADIR) d;
    ADIR SD = D->subdir;
    ADIR sd = NULL;

    while(SD)
    {
        if(strcmp(SD->nume, nume) == 0)
            break;
        sd = SD;
        if(strcmp(SD->nume, nume) < 0)
            SD = SD->dr;
        else
            SD = SD->st;
    }

    if(!SD)
    {
        printf("Directory %s doesn't exist!\n", nume);
        return;
    }
    

    if(!SD->st && !SD->dr)
    {
        if(!sd)
        {
            distr_arb_fis(SD->fisier);
            distr_arb_dir(SD->subdir);
            D->subdir = NULL;
        }
        else
        {
            if(sd->st)
                if(strstr(sd->st->nume, nume))
                {
                    distr_arb_dir(sd->st->subdir);
                    distr_arb_fis(sd->st->fisier);
                    sd->st = NULL;
                }
            if(sd->dr)
                if(strstr(sd->dr->nume, nume))
                {
                    distr_arb_dir(sd->dr->subdir);
                    distr_arb_fis(sd->dr->fisier);
                    sd->dr = NULL;
                }
        }
        free(SD->nume);
        free(SD);
        return;
    }

    if(!SD->st)
    {
        ADIR aux = SD->dr;
        stpcpy(SD->nume, SD->dr->nume);
        SD->st = SD->dr->st;
        SD->dr = SD->dr->dr;
        distr_arb_fis(aux->fisier);
        distr_arb_dir(aux->subdir);
        free(aux->nume);
        free(aux);
        return;
    }

    if(!SD->dr)
    {
        ADIR aux = SD->st;
        strcpy(SD->nume, SD->st->nume);
        SD->dr = SD->st->dr;
        SD->st = SD->st->st;
        distr_arb_fis(aux->fisier);
        distr_arb_dir(aux->subdir);
        free(aux->nume);
        free(aux);
        return;
    }

    ADIR aux1 = SD->dr;
    ADIR aux2 = SD->dr->st;

    sd = SD->st;
    while(SD)
    {
        if(!sd->dr)
        {
            sd->dr = aux2;
            break;
        }
        sd = sd->dr;
    }

    SD->dr->st = NULL;
    strcpy(SD->nume, SD->dr->nume);
    SD->dr = SD->dr->dr;

    distr_arb_fis(aux1->fisier);
    distr_arb_dir(aux1->subdir);

    free(aux1->nume);
    free(aux1);


}

void* cd(void* d, char* nume) //Functie ce trece dintr-un director in altul
{
    ADIR D = (ADIR) d;

    if(strstr(nume, "..")) //Trecem la directorul parinte
    {
        void *nd = (void*) D->parinte;
        return nd;
    }
    

    if(D->subdir)  //Cautare director
    {
        ADIR SD = D->subdir;

        while(SD)
        {
            if(strcmp(SD->nume, nume) == 0)//Trecem in directorul cautat
            {
                void *nd = (void*) SD;
                return nd;
            }

            if(strcmp(SD->nume, nume) < 0)
                SD = SD->dr;
            else
                SD = SD->st;
        }
    }

    //Directorul cautat nu exista
    printf("Directory not found!\n");
    return d;
}

void pwd(void *d) //Afisam calea pana la directorul curent
{
    ADIR D = (ADIR) d;
    
    if(D->parinte)
        pwd((void*)D->parinte);

    printf("/%s", D->nume);
}

void find_fis(void* d, char *nume, void **fis) //Functie ce cauta daca un fisier exista
{
    ADIR D = (ADIR) d;
    if(!D)
        return;

    AFIS F = D->fisier;

    while(F) //Cautare fisier in arborele de fisiere
    {
        if(strcmp(F->nume, nume) == 0)
        {
            *fis = d;
            return;
        }
        if(strcmp(F->nume, nume) < 0)
            F = F->dr;
        else
            F = F->st;
    }

    if(D) //trecere prin toate directoarele
    {
        if(D->st)
            find_fis((void*) D->st, nume, fis);
        
        if(D->dr)
            find_fis((void*) D->dr, nume, fis);
    }

    ADIR SD = D->subdir;
    find_fis((void*) SD, nume, fis);
}

void find_dir(void* d, char *nume, void **dir) //Functie ce cauta daca un director exista
{
    ADIR D = (ADIR) d;
    if(!D)
        return;
    ADIR SD = D->subdir;

    while(D) //Cautam in toate directoarele
    {
        if(strcmp(D->nume, nume) == 0)
        {

            *dir = (void*)D;
            return;
        }
        if(strcmp(D->nume, nume) < 0)
        {
            find_dir((void*)D->dr, nume, dir);
            D = D->dr;
        }
        else
        {
            find_dir((void*)D->st, nume, dir);
            D = D->st;
        }
    }
    
    find_dir((void*)SD, nume, dir);
}

void quit(void *d) //Functie ce distruge toti arborii si opreste programul
{
    ADIR D = (ADIR) d;

    if(!D)
        return;

    distr_arb_dir(D);
}

int main()
{
    //Creare directorul original 'root'
    void *D = malloc(sizeof(Dir));
    ((ADIR) D)->nume = malloc(51);
    strcpy(((ADIR) D)->nume, "root");
    ((ADIR) D)->fisier = NULL;
    ((ADIR) D)->parinte = NULL;
    ((ADIR) D)->st = NULL;
    ((ADIR) D)->dr = NULL;
    ((ADIR) D)->subdir = NULL;

    void* root = D;

    char *intrare = malloc(100);

    while(fgets(intrare, 100 * sizeof(char), stdin)) //Citire task-uri
    {
        if(intrare[strlen(intrare) - 1] == 10)
            intrare[strlen(intrare) - 1] = 0;

        char *functie = strtok(intrare, " ");
        if(!functie)
            break;
        
        if(strstr(functie, "touch"))
        {
            char *nume = strtok(NULL, " ");
            touch(D, nume);
        }
        
        if(strstr(functie, "mkdir"))
        {
            char *nume = strtok(NULL, " ");
            mkdir(D, nume);
        }

        if(strstr(functie, "ls"))
        {
            ls(D);
        }

        if(!strcmp(functie, "rm"))
        {
            char *nume = strtok(NULL, " ");
            rm(D, nume);
            
        }

        if(!strcmp(functie, "rmdir"))
        {
            char *nume = strtok(NULL, " ");
            rmdir(D, nume);
        }

        if(strstr(functie, "cd"))
        {
            char *nume = strtok(NULL, " ");
            D = cd(D, nume);
        }


        if(strstr(functie, "pwd"))
        {
            pwd(D);
            printf("\n");   
        }

        if(strstr(functie, "find"))
        {
            void* d = NULL;
            char *nume = strtok(NULL, " ");
            if(!strcmp(nume, "-f"))
            {
                nume = strtok(NULL, " ");
                find_fis(root, nume, &d);
                if(!d)
                    printf("File %s not found!\n", nume);
                else
                {
                    printf("File %s found!\n", nume);
                    pwd(d);
                    printf("\n");
                }
                    
            }
            else
            {
                nume = strtok(NULL, " ");
                find_dir(root, nume, &d);
                if(!d)
                    printf("Directory %s not found!\n", nume);
                else
                {
                    printf("Directory %s found!\n", nume);
                    pwd(d);
                    printf("\n");
                }
                    
            }
        }

        if(strstr(functie, "quit"))
        {
            quit(root);
            free(intrare);
            break;   
        }




    }

    return 0;
}
