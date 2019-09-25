//Guilherme Dall'Agnol Deconto
//Gustavo Possebon Machado
//Bernardo de Cesaro
//Pontifícia Universidade Católica do Rio Grande do Sul
//Sistemas Operacionais

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define portions 10 // ammount of portions the cook makes
#define numCanibals 5 // cannibals ammount
#define cookTime 5 // time for the food to be ready
#define eatTime 1 // time it takes for a cannibal to eat
int initialPortions = 10; // ammount of initial portions
int flag[2];
int turn;
const int MAX = 1e9;
int res = 0;

pthread_mutex_t cook = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t row =  PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t serv = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foodReady = PTHREAD_MUTEX_INITIALIZER;

void *Cooking(){
    int light; //controle if the mutex will be available or not
    while (1) {
        pthread_mutex_lock(&row);
        printf("Mutex row locked\n");
        if (initialPortions > 0) { //a loop that will run until it have no more food
            printf("Canibal is eating before else\n Portion number : %d\n", initialPortions);
            initialPortions --; //decrease the amount every time a Canibal eats
            sleep(eatTime);
        }else{
            printf("Call the cook\n");
            pthread_mutex_lock(&serv);
            pthread_mutex_unlock(&cook);
            pthread_mutex_lock(&foodReady);
            printf("Canibal eating after else\n");
            initialPortions--;
            pthread_mutex_unlock(&serv);
        }
        printf("Canibal is ready\n");
        fflush(stdout);
        pthread_mutex_unlock(&row);
    }
}

void *Cook(){
    while (1) {
        pthread_mutex_lock(&cook); //It's time for the mutex of the cook
        printf("Calling the Cook, he is cooking....\n");
        initialPortions = portions; //set the initial portions to be the same as 10 (always)
        sleep(cookTime); //make it to sleep for a short time until called again
        printf("Cook is making food\n");
        fflush(stdout);
        pthread_mutex_unlock(&foodReady);
    }
}

void lock_init() {
    // Inicializa o lock resetando as duas threads e indicando o turno para uma delas
    flag[0] = flag[1] = 0;
    turn = 0;
}

// Executa antes de entrar na sessão crítica
void lock(int victim) {
    // Seta em 1 para dizer que ele quer "adquirir"
    flag[victim] = 1;
    
    // Mas primeiro dá a chance para a outra thread de também realizar a operação
    turn = 1-victim;
    
    // Fica em espera até que seja a sua vez e que a thread y não queira entrar;
    while (flag[1-victim]==1 && turn==1-victim) ;
}

// Executa depois de sair da sessão crítica
void unlock(int victim){
    flag[victim] = 0;
}

void* func(void *s)
{
    int i = 0;
    int victim = (int *)s;
    printf("Thread Entered: %d\n", victim);
    
    lock(victim);
    
    // Sessão crítica (Só uma das threads irá entrar aqui)
    for (i=0; i<MAX; i++)
        res++;
    
    unlock(victim);
}


int main(){
    int rc;
    pthread_t canibal[numCanibals];
    pthread_t cook;
    lock_init(); //2º exercício
    pthread_mutex_lock(&cook);
    pthread_mutex_unlock(&serv);
    pthread_mutex_unlock(&row);
    pthread_mutex_lock(&foodReady);
    for (int i = 0; i < numCanibals; i++) {
        rc = pthread_create(&canibal[i], NULL, Cooking, NULL); //1º exercício
        pthread_create(&canibal[i],NULL,func,(void*)0); //2º exercício
        printf("Creating canibal %d\n", i);
        if(rc){
            printf("Error creating Canibal Thread : %d\n", i);
        }
    }
    rc = pthread_create(&cook, NULL, Cook, NULL); //1º exercício
    pthread_create(&cook,NULL,func, (void*)1); //2º exercício
    printf("Hiring cook\n");
    if(rc){
        printf("Error creating Canibal Thread\n");
    }
    pthread_join(cook, NULL);

    printf("Actual Count: %d | Expected Count: %d\n", res, MAX*2); //2º exercício
    return 0;
}
