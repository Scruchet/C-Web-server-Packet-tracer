#include "libreseau.h"

sem_t semaphore;

int initialisationServeur(char* service, int connexions)
{
    struct addrinfo precisions, *resultat, *origine;
    int statut;
    int s;

    /* Construction de la structure adresse */
    memset(&precisions, 0, sizeof precisions);
    precisions.ai_family = AF_UNSPEC;
    precisions.ai_socktype = SOCK_STREAM;
    precisions.ai_flags = AI_PASSIVE;
    statut = getaddrinfo(NULL, service, &precisions, &origine);
    if (statut < 0) {
        perror("initialisationServeur.getaddrinfo");
        exit(EXIT_FAILURE);
    }
    struct addrinfo* p;
    resultat = origine;
    for (p = origine; p != NULL; p = p->ai_next) { // Parcourt la liste chainée pour trouver l'addresse
        if (p->ai_family == AF_INET6) {
            resultat = p; // Stock l'adresse dans la variable resultat
            break;
        }
    }

    /* Creation d'une socket */
    s = socket(resultat->ai_family, resultat->ai_socktype, resultat->ai_protocol); // Création socket avec l'adresse "resultat"
    if (s < 0) { // si diff de 0 erreur
        perror("initialisationServeur.socket");
        exit(EXIT_FAILURE);
    }

    /* Options utiles */
    int vrai = 1;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &vrai, sizeof(vrai)) < 0) {

        perror("initialisationServeur.setsockopt (REUSEADDR)");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &vrai, sizeof(vrai)) < 0) {

        perror("initialisationServeur.setsockopt (NODELAY)");
        exit(EXIT_FAILURE);
    }

    /* Specification de l'adresse de la socket */
    statut = bind(s, resultat->ai_addr, resultat->ai_addrlen);
    if (statut < 0) {
        freeaddrinfo(origine);
        shutdown(s, SHUT_RDWR);
        return -1;
    }

    /* Liberation de la structure d'informations */
    freeaddrinfo(origine);

    /* Taille de la queue d'attente */
    statut = listen(s, connexions);
    if (statut < 0) {
        shutdown(s, SHUT_RDWR);
        return -2;
    }
    return s;
}

void* gestionThread(void* arg)
{

    int dialogue = ((arg_t*)arg)->dialogue;
    int ecoute = ((arg_t*)arg)->ecoute;
    int (*traitement)(int) = ((arg_t*)arg)->traitement;
    free(arg);

    /* Attente de 10 secondes pour les tests*/
    sleep(DELAY_SLEEP);

    /* Passage de la socket de dialogue a la fonction de traitement */
    if (traitement(dialogue) < 0) {
        shutdown(ecoute, SHUT_RDWR);
    }
    sem_post(&semaphore);
    pthread_exit(NULL);
}

int boucleServeur(int ecoute, int (*traitement)(int))
{
    pthread_t thread;
    int dialogue;

    if (sem_init(&semaphore, 0, MAX_CONNEXIONS) == -1) {
        perror("boucleServeur.sem_init");
        exit(EXIT_FAILURE);
    }

    while (1) {
        /* Attente d'une connexion */
        if ((dialogue = accept(ecoute, NULL, NULL)) < 0) {
            return -1;
        }

        /* permet de faire attendre si il n'y a
         * plus de connexions possibles */
        sem_wait(&semaphore);

        /* Gestion du nouveau thread */
        arg_t* arg = malloc(sizeof(arg_t*));
        arg->ecoute = ecoute;
        arg->dialogue = dialogue;
        arg->traitement = traitement;
        pthread_create(&thread, NULL, gestionThread, (void*)arg);
    }
}
