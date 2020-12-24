#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

struct args_t  {
    char *tampon;
    unsigned long taille;
    unsigned long cptr;
};

unsigned long compteur_gc(char *bloc, unsigned long taille) {
    unsigned long i, cptr = 0;

    for (i = 0; i < taille; i++)
        if (bloc[i] == 'G' || bloc[i] == 'C')
            cptr++;

    return cptr;
}

void* tcompteur_gc(void *arg) {
    struct args_t *a = (struct args_t *) arg;
    a->cptr = compteur_gc(a->tampon, a->taille);
    return NULL;
}

int main(int argc, char *argv[]) {
    struct stat st;
    int fd;
    char *tampon;
    int lus;
    unsigned long cptr = 0;
    off_t taille = 0;
    struct timespec debut, fin;
    int boucle = 0;
    int nbThreads = atoi(argv[2]);

    assert(argc > 1);

    /* Quelle taille ? */
    assert(stat(argv[1], &st) != -1);
    tampon = malloc(st.st_size);
    assert(tampon != NULL);

    /* Chargement en mémoire */
    fd = open(argv[1], O_RDONLY);
    assert(fd != -1);
    while ((lus = read(fd, tampon + taille, st.st_size - taille)) > 0)
        taille += lus;
    assert(lus != -1);
    assert(taille == st.st_size);
    close(fd);

    /* Calcul proprement dit */
    assert(clock_gettime(CLOCK_MONOTONIC, &debut) != -1);
    //  Création et remplissage des données et structures
    pthread_t tid[nbThreads];
    int r[nbThreads];
    struct args_t a[nbThreads];
    int sizePart = taille / nbThreads;
    for(boucle = 0; boucle < nbThreads - 1; boucle++) {
        a[boucle].tampon = &tampon[sizePart*boucle];
        a[boucle].taille = sizePart;
    }
    a[nbThreads-1].tampon = &tampon[sizePart*(nbThreads-1)];
    a[nbThreads-1].taille = sizePart + (taille - sizePart * nbThreads);
    //  Création et lancement des threads et récupération des résultats
    for(boucle = 0; boucle < nbThreads; boucle++) {
        pthread_create(&tid[boucle], NULL, &tcompteur_gc, &a[boucle]);
    }
    for(boucle = 0; boucle < nbThreads; boucle++) {
        r[boucle] = pthread_join(tid[boucle], NULL);
    }
    //  Calcul des données d'affichage des résultats des threads
    for(boucle = 0; boucle < nbThreads; boucle++) {
        cptr = cptr + a[boucle].cptr;
        assert(r[boucle] ==0);
    }
    assert(clock_gettime(CLOCK_MONOTONIC, &fin) != -1);
    //  Calcul du temps d'exécution
    fin.tv_sec  -= debut.tv_sec;
    fin.tv_nsec -= debut.tv_nsec;
    if (fin.tv_nsec < 0) {
        fin.tv_sec--;
        fin.tv_nsec += 1000000000;
    }
    printf("%ld.%09ld\n", fin.tv_sec, fin.tv_nsec);

    return 0;
}