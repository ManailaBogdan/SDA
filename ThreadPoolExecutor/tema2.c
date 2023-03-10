/*Manaila Bogdan - 313CB*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Cozi.h"
#include "Stive.h"


//Functie pentru adaugarea unui task in Coada de asteptare
void add_task(FILE *f, void *CW, int nr, int time, int pri, int id[32768]){

    for(int i = 1; i <= nr; i ++)
    {
        Task *t = malloc(sizeof(Task)); //alocare spatiu pentru un task

        for(int j = 1; j <= 32767; j ++) //atribuire id, prioritate si timp de executie pt un task
        {
            if(id[j] == 0)
            {
                t->id = j;
                t->time = time;
                t->pri = pri;
                t->Thread = -1;
                t->time_ex = 0;
                id[j] = 1;
                break;
            }
        }

        if(!((AC)CW)->sc) //adaugare primului task din coada
            IntrQ(CW, (void*)t);

        else //adaugarea unui element in coada dupa ordinea ceruta
        {
            Task *aux;
            int ok = 0;
            void *C_aux = InitQ();
            while(((AC)CW)->sc)
            {   
                aux = malloc(sizeof(Task));
                ExtrQ(CW, (void*)aux);
                if(aux->pri < t->pri)
                {
                    IntrQ(C_aux, (void*)t);
                    IntrQ(C_aux, (void*)aux);
                    if(((AC)CW)->sc)
                        ConcatQ(C_aux, CW);
                    MutaQ(&CW, C_aux);
                    ok = 1;
                    break;
                }

                else if(aux->pri == t->pri)
                {
                    if(aux->time > t->time)
                    {
                        IntrQ(C_aux, (void*)t);
                        IntrQ(C_aux, (void*)aux);
                        if(((AC)CW)->sc)
                            ConcatQ(C_aux, CW);
                        MutaQ(&CW, C_aux);
                        ok = 1;
                        break;
                    }

                    else if(aux->time == t->time)
                    {
                        if(aux->id > t->id)
                        {
                            IntrQ(C_aux, (void*)t);
                            IntrQ(C_aux, (void*)aux);
                            if(((AC)CW)->sc)
                                ConcatQ(C_aux, CW);
                            MutaQ(&CW, C_aux);
                            ok = 1;
                            break;
                        }
                    }
                }

                IntrQ(C_aux, aux);
            }
            
            if(!ok)
            {
                IntrQ(CW, (void*)t);
                ConcatQ(C_aux,CW);
                MutaQ(&CW, C_aux);
            }

            free(C_aux);
            
        }

        fprintf(f, "Task created successfully : ID %d.\n", t->id);
    }
}

//Functie pentru a vedea starea unui task
void get_task(FILE *f, void *CW, void *CR, void *CF, int id){

    Task *aux;
    int ok = 0;
    if(((AC)CR)->sc) //Verifica daca task-ul ruleaza
    {
        void *C_aux = InitQ();
        while(((AC)CR)->sc)
        {
            aux = malloc(sizeof(Task));
            ExtrQ(CR, (void*)aux);
            if(aux->id == id && !ok)
            {
                fprintf(f, "Task %d is running (remaining_time = %d).\n", id, aux->time - aux->time_ex);
                ok=1;
            }

            IntrQ(C_aux, (void*)aux);
        }

        MutaQ(&CR, C_aux);
        free(C_aux);
        
    }


    if(((AC)CW)->sc && !ok) //verifica daca task-ul e in asteptare
    {
        void *C_aux = InitQ();
        while(((AC)CW)->sc)
        {
            aux = malloc(sizeof(Task));
            ExtrQ(CW, (void*)aux);
            
            if(aux->id == id && !ok)
            {
                fprintf(f, "Task %d is waiting (remaining_time = %d).\n", id, aux->time);
                ok = 1;
            }

            IntrQ(C_aux, (void*)aux);
        }
        MutaQ(&CW, C_aux);
        free(C_aux);
    }
    
    if(((AC)CF)->sc && !ok) //verifica daca task-ul este finalizat
    {
        void *C_aux = InitQ();
        while(((AC)CF)->sc)
        {
            aux = malloc(sizeof(Task));
            ExtrQ(CF, (void*)aux);
            if(aux->id == id && !ok)
            {
                fprintf(f, "Task %d is finished (executed_time = %d).\n", id, aux->time);
                ok = 1;
            }

            IntrQ(C_aux, (void*)aux);
        }

        MutaQ(&CF, C_aux);
        free(C_aux);
    }
    if(!ok) //Task-ul nu exista
    {
        fprintf(f, "Task %d not found.\n", id);
    }
   
}

//Functie pentru a vedea starea unui thread
void get_thread(FILE *f, void *S, void *CR, int id){

    void *S_aux = InitS();
    void *el;
    int ok = 0;
    
    while(((AS)S)->vs) //Verificare daca thread-ul este liber
    {
        el = malloc(sizeof(int));
        Pop(S, el);
        
        if(*(int*)el == id)
        {
            fprintf(f, "Thread %d is idle.\n", id);
            ok = 1;
        }

        Push(S_aux, el);
    }

    while(((AS)S_aux)->vs)
    {
        el = malloc(sizeof(int));
        Pop(S_aux, el);
        Push(S, el);
    }

    free(S_aux);

    if(!ok) //Verificare daca thread-ul ruleaza un task
    {   
        Task *aux;
        void *C_aux = InitQ();
        while(((AC)CR)->sc)
        {
            aux = malloc(sizeof(Task));
            ExtrQ(CR, (void*)aux);
           
            if(aux->Thread == id)
            {
                fprintf(f, "Thread %d is running task %d (remaining_time = %d).\n", id, aux->id, aux->time - aux->time_ex);
            }

            IntrQ(C_aux, (void*)aux);
        }

        MutaQ(&CR, C_aux);
        free(C_aux);
    }
}

//Functie pentru printarea unei cozi
void print(FILE *f, void *CR, void *CW, void *CF, char *tip){

    Task *aux;
    if(strstr(tip, "waiting")) //Printarea cozii de asteptare
    {
        fprintf(f, "====== Waiting queue =======\n[");
        if(((AC)CW)->sc)
        {
            void *C_aux = InitQ();
            while(((AC)CW)->sc)
            {
                aux = malloc(sizeof(Task));
                ExtrQ(CW, (void*)aux);
                
                fprintf(f, "(%d: priority = %d, remaining_time = %d)", aux->id, aux->pri, aux->time - aux->time_ex);
                if(((AC)CW)->sc)
                    fprintf(f, ",\n");

                IntrQ(C_aux, (void*)aux);
            }
            MutaQ(&CW, C_aux);
            free(C_aux);
        }
    }

    if(strstr(tip, "running")) //Printarea cozii de rulare
    {
        fprintf(f, "====== Running in parallel =======\n[");
        if(((AC)CR)->sc)
        {
            void *C_aux = InitQ();
            while(((AC)CR)->sc)
            {
                aux = malloc(sizeof(Task));
                ExtrQ(CR, (void*)aux);
                
                fprintf(f, "(%d: priority = %d, remaining_time = %d, running_thread = %d)", aux->id, aux->pri, aux->time - aux->time_ex, aux->Thread);
                if(((AC)CR)->sc)
                    fprintf(f, ",\n");

                IntrQ(C_aux, (void*)aux);
            }
            MutaQ(&CR, C_aux);
            free(C_aux);
        }
    }

    if(strstr(tip, "finished")) //Printarea cozii finished
    {
        fprintf(f, "====== Finished queue =======\n[");
        if(((AC)CF)->sc)
        {
            void *C_aux = InitQ();
            while(((AC)CF)->sc)
            {
                aux = malloc(sizeof(Task));
                ExtrQ(CF, (void*)aux);
                
                fprintf(f, "(%d: priority = %d, executed_time = %d)", aux->id, aux->pri, aux->time);
                if(((AC)CF)->sc)
                    fprintf(f, ",\n");

                IntrQ(C_aux, (void*)aux);
            }
            MutaQ(&CF, C_aux);
            free(C_aux);
        }
    }

    fprintf(f,"]\n");
}

//Functia de rulare pentru un anumit timp
void run(FILE *f, void *CR, void *CW, void *CF, void *S, int *t, int Q, int id[32768], int *total){

    fprintf(f, "Running tasks for %d ms...\n", *t);
    void *aux2;
    int t_max;

    for(int i = 0; i <= *t; i = i + Q) //rulare cu pas Q
    {
       
        int t_ex;
        if(*t - i < Q)
            t_ex = *t - i;
        else
            t_ex = Q;

        t_max = 0;
       
        if(!((AC)CW)->sc && !((AC)CR)->sc) //Verificare daca exista task-uri in asteptare/ rulare
            break;

        Task *aux1;
        void *C_aux = InitQ();

        while(((AS)S)->vs && ((AC)CW)->sc) //Asociere thread - task si adaugare in coada running
        {
            aux1 = malloc(sizeof(Task));
            aux2 = malloc(sizeof(int));
            ExtrQ(CW, (void*)aux1);
            Pop(S, aux2);

            aux1->Thread = *(int*)aux2;  

            free(aux2);
            IntrQ(CR, (void*)aux1);
        }
            
        while(((AC)CR)->sc) //rulare
        {
            aux1 = malloc(sizeof(Task));
            aux2 = malloc(sizeof(int));

            ExtrQ(CR, (void*)aux1);

            if(aux1->time - aux1->time_ex > t_max) //verificare timp de rulare maxim
            {
                if(aux1->time - aux1->time_ex < t_ex)
                    t_max = aux1->time - aux1->time_ex;
                else
                    t_max = t_ex;
            }

            if(aux1->time - aux1->time_ex < t_ex) //rulare task
                aux1->time_ex = aux1->time;
            else
                aux1->time_ex += t_ex; 
             
            if(aux1->time == aux1->time_ex) //verificare daca task-ul este finalizat
            {
                id[aux1->id] = 0;
                IntrQ(CF, (void*)aux1);
                *(int*)aux2 = aux1->Thread;
                Push(S, aux2);
            }
            else
            {
                free(aux2);
                IntrQ(C_aux, (void*)aux1);
            }

        }

        if(((AC)C_aux)->sc)
            MutaQ(&CR, C_aux);
        free(C_aux);
    
        C_aux = InitQ();
        while(((AS)S)->vs && ((AC)CW)->sc) //Adaugare task-uri in running daca este cazul
        {
            aux1 = malloc(sizeof(Task));
            aux2 = malloc(sizeof(int));
    
            ExtrQ(CW, (void*)aux1);
            Pop(S, aux2);
            aux1->Thread = *(int*)aux2;

            free(aux2);
            IntrQ(CR, (void*)aux1);
        }

        free(C_aux);
        (*total) += t_max; //incrementare timp total de rulare
    }

}

//Functie pentru rularea tuturor task-urilor ramase
void finish(FILE *f, void *CR, void *CW, void *CF, void *S, int Q, int *total){

    while(((AC)CW)->sc || ((AC)CR)->sc) //Verificare daca exista task-uri in asteptare/ rulare
    {
        Task *aux1;
        void *C_aux = InitQ();
        void *aux2;
      
        int t_max = 0;
        while(((AS)S)->vs && ((AC)CW)->sc) //Asociere thread - task si adaugare in coada running
        {
            aux1 = malloc(sizeof(Task));
            aux2 = malloc(sizeof(int));
            ExtrQ(CW, (void*)aux1);
            Pop(S, aux2);
            aux1->Thread = *(int*)aux2;  

            free(aux2);
            IntrQ(CR, (void*)aux1);
        }
            
        while(((AC)CR)->sc) //rulare task-uri
        {
            aux1 = malloc(sizeof(Task));
            aux2 = malloc(sizeof(int));

            ExtrQ(CR, (void*)aux1);

            if(aux1->time - aux1->time_ex > t_max) //verificare timp maxim de rulare
            {
                if(aux1->time - aux1->time_ex < Q)
                    t_max = aux1->time - aux1->time_ex;
                else
                    t_max = Q;
            }

            if(aux1->time - aux1->time_ex < Q) //rulare task 
                aux1->time_ex = aux1->time;
            else
                aux1->time_ex += Q; 
             
            if(aux1->time == aux1->time_ex) //verificare daca task-ul e finalizat
            {
                IntrQ(CF, (void*)aux1);
                *(int*)aux2 = aux1->Thread;
                Push(S, aux2);
            }
            else
            {
                free(aux2);
                IntrQ(C_aux, (void*)aux1);
            }

        }

        if(((AC)C_aux)->sc)
            MutaQ(&CR, C_aux);
        free(C_aux);

        C_aux = InitQ();
        while(((AS)S)->vs && ((AC)CW)->sc)
        {
            aux1 = malloc(sizeof(Task));
            aux2 = malloc(sizeof(int));
    
            ExtrQ(CW, (void*)aux1);
            Pop(S, aux2);
            aux1->Thread = *(int*)aux2;
    
            IntrQ(CR, (void*)aux1);
        }
        free(C_aux);
        (*total) += t_max;

    }

    fprintf(f, "Total time: %d", *total);

}

int main(int argc, char* argv[]){

    FILE *fi, *fo;
    char *line = NULL;
    size_t len = 0;

    int id[32768]; //Initializare vector de frecventa pentru id-uri
    for(int i = 0; i < 32768; i ++)
        id[i] = 0;

    fi = fopen(argv[1], "rt"); //Deschidere fisier input
    if(!fi)
        printf("\nERR fi!!\n");

    fo = fopen(argv[2], "wt"); //Deschidere fisier output
    if(!fo)
        printf("\nERR fo!!\n");

    int Q, C, t = 0, total = 0; //Citire cuata de timp si numar coruri
    fscanf(fi, "%d%d", &Q, &C);

    void *S = InitS(); 

    for(int i = 2 * C - 1; i >= 0; i --) //Adaugare id-uri pentru thread-uri
    {
        void *j = malloc(sizeof(int));
        memcpy(j, &i, sizeof(int));
        Push(S, j);
    }

    
    void *CW = InitQ();
    void *CR = InitQ();
    void *CF = InitQ();
    
    while(getline(&line, &len, fi) != -1) //parcurgere fisier input
    {
        char *functie = strtok(line, " ");
        if(functie[strlen(functie) - 1] == 10)
            functie[strlen(functie) - 1] = 0;

        if(strstr(functie, "add_task"))
        {
           int nr = atoi(strtok(NULL, " "));
           int time = atoi(strtok(NULL, " "));
           int pri = atoi(strtok(NULL, " "));
           add_task(fo, CW, nr, time, pri, id);
        }

        if(strstr(functie, "get_task"))
        {
           int nr = atoi(strtok(NULL, " "));
           get_task(fo, CW, CR, CF, nr);
        }

        if(strstr(functie, "get_thread"))
        {
           int nr = atoi(strtok(NULL, " "));
           get_thread(fo, S, CR, nr);
        }

        if(strstr(functie, "print"))
        {
           char* c = strtok(NULL, " ");
           print(fo, CR, CW, CF, c);
        }

        if(strstr(functie, "run"))
        {
           t = atoi(strtok(NULL, " "));
           run(fo, CR, CW, CF, S, &t, Q, id, &total);
        }

        if(strstr(functie, "finish"))
        {
            finish(fo, CR, CW, CF, S, Q, &total);
        }

        functie = strtok(NULL, " ");
    }
    
    DistrQ(&CW);
    DistrQ(&CR);
    DistrQ(&CF);
    DistrS(&S);

    fclose(fi);
    fclose(fo);
    free(line);
    return 0;
}