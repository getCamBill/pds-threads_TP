#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include <pthread.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <semaphore.h>


#define NB_THREADS 8
sem_t semTab[NB_THREADS];
struct args_t  {
    int yoda;
};

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

void *p(void *arg) {
    struct args_t *y = (struct args_t *) arg;
    a(y->yoda);
    int i = 0;
    for(i = 0 ; i < NB_THREADS-1 ; i++) {
        assert(sem_post(&semTab[y->yoda]) == 0);
    }
    //On attend le go
    for(i = 0 ; i < NB_THREADS; i++) {
        if(i != y->yoda) { assert(sem_wait(&semTab[i]) == 0); }
    }
    b(y->yoda);
    return NULL;
}

int main() {

    pthread_t tid[NB_THREADS];
    struct args_t o[NB_THREADS];

    for(int l = 0; l < NB_THREADS; l++) {
        o[l].yoda = l;
        assert(sem_init(&semTab[l], 0, 0) == 0);
    }
    //  Création et lancement des threads et récupération des résultats
    for(int l = 0; l < NB_THREADS; l++) {
        assert(pthread_create(&tid[l], NULL, p, &o[l])==0);
    }
    for(int l = 0; l < NB_THREADS; l++) {
        assert(pthread_join(tid[l], NULL)==0);
    }

    for(int l = 0; l < 2; l++) {
        sem_destroy(&semTab[l]);
    }

    return 0;
}
