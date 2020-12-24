#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>


#define NB_THREADS 8
sem_t sem1, sem2;
int force = 0;
struct args_t  {
    int yoda;
};
//--------------------------
void a(int i) {
    sleep(0);
    printf("a%d\n", i);
    sleep(0);
}
void b(int i) {
    sleep(0);
    printf("b%d\n", i);
    sleep(0);
}
//--------------------------
void *p(void *arg) {
    struct args_t *y = (struct args_t *) arg;

    a(y->yoda);

    sem_post(&sem2);
    force++;

    if (NB_THREADS == force) {
        sem_post(&sem1);// les threads sont tous là
    }
    sem_wait(&sem1);
    sem_wait(&sem2);
    // on met les threads en attente
    
    // on lance les threds un par un
    sem_post(&sem1);

    b(y->yoda);

    return NULL;
}
//--------------------------
int main() {

    pthread_t tid[NB_THREADS];
    struct args_t o[NB_THREADS];
    //--------------------------
    for(int l = 0; l < NB_THREADS; l++) {
        o[l].yoda = l;
    }
    sem_init(&sem1,0,0);
    sem_init(&sem2,0,0);
    //--------------------------
    //  Création et lancement des threads et récupération des résultats
    for(int l = 0; l < NB_THREADS; l++) {
        assert(pthread_create(&tid[l], NULL, p, &o[l])==0);
    }

    for(int l = 0; l < NB_THREADS; l++) {
        assert(pthread_join(tid[l], NULL)==0);
    }

    // Les sémaphores consomment des ressources qui peuvent être limitées dans certains environnements. 
    //Il est important de détruire proprement les sémaphores dès qu’ils ne sont plus nécessaires.
    sem_destroy(&sem1);
    sem_destroy(&sem2);


    return 0;
}
