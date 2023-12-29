#ifndef ANALYSTE_HTTP
#define ANALYSTE_HTTP

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WEB_DIR "./www"
#define PAGE_NOTFOUND "error.html"
#define EXTENTION ".html"
#define MAX_BUFFER 1024
#define CODE_OK 200
#define CODE_NOTFOUND 404
#define CODE_NOTOK 501
#define PAGE_HOME "formulaire.html"
#define PAGE_REPONSE "reponse.html"
#define FICHIER_CSV "reponse.csv"

int envoyer_html(FILE* dialogue, char* htmlPath);
int gestionhttp(FILE* dialogue);

#endif
