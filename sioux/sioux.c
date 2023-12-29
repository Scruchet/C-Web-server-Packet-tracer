#include "analyste_http.h"
#include "libreseau.h"
#include <getopt.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

char service[MAX_PORT_LENGTH];
int port = NM_PORT;

int gestionClient(int s)
{
    printf("Nouvelle connexion !\n");
    FILE* dialogue = fdopen(s, "a+");
    if (dialogue == NULL) {
        perror("gestionClient.fdopen");
        exit(EXIT_FAILURE);
    }

    gestionhttp(dialogue);
    fclose(dialogue);
    return 0;
}

void analyseArguments(int argc, char* argv[])
{

    struct option long_options[] = {
        { "port", required_argument, NULL, 'p' },
        { 0, 0, 0, 0 }
    };

    int option;
    int option_index = 0;

    while ((option = getopt_long(argc, argv, "p:", long_options, &option_index)) != -1) {
        switch (option) {
        case 'p':
            // L'option -p (--port) a été spécifiée avec un argument
            port = atoi(optarg);
            break;

        default:
            // Gérer d'autres options ou erreurs ici
            break;
        }
    }
    printf("Port spécifié : %d\n", port);
}

int main(int argc, char* argv[])
{
    int socket;
    /* Lecture des arguments de la commande */
    analyseArguments(argc, argv);
    sprintf(service, "%d", port);

    /* Initialisation du serveur */
    socket = initialisationServeur(service, MAX_CONNEXIONS);
    if (socket < 0) {
        fprintf(stderr, "Port non utilisable\n");
        exit(EXIT_FAILURE);
    }

    printf("Serveur à l'écoute sur le port n° %d\n", port);
    /* Lancement de la boucle d'écoute */
    boucleServeur(socket, gestionClient);

    return 0;
}
