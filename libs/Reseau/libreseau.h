#ifndef LIBRESEAU_H
#define LIBRESEAU_H

#include <getopt.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <fcntl.h>
#include <sys/stat.h>

#define MAX_BUFFER 1024
#define MAX_CONNEXIONS 3

#define NM_PORT 8080
#define MAX_PORT_LENGTH 10

#define WEB_DIR "./www"
#define PAGE_NOTFOUND "error.html"

#define CODE_OK 200
#define CODE_NOTFOUND 404

#define DELAY_SLEEP 0

typedef struct {
    int ecoute;
    int dialogue;
    int (*traitement)(int);
} arg_t;

int initialisationServeur(char* service, int connexions);
int boucleServeur(int ecoute, int (*traitement)(int));
void* gestionThread(void* arg);

#endif
