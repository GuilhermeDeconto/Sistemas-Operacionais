//Guilherme Dall'Agnol Deconto
//Gustavo Possebon Machado
//Bernardo de Cesaro
//Pontifícia Universidade católica do Rio Grande do Sul
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

pthread_mutex_t cook =     PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t row =    PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t serv =   PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t foodReady =   PTHREAD_MUTEX_INITIALIZER;


int main(){
    
}
