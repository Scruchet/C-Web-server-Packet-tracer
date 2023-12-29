#include "analyste_http.h"

int envoyer_html(FILE* dialogue, char* fichier)
{
    int code = CODE_OK;
    FILE* html = fopen(fichier, "r");
    if (html == NULL) {
        perror("Pas de fichier trouvé");
        fprintf(dialogue, "HTTP/1.1 %d\r\n\r\n", CODE_NOTFOUND);
        return -1;
    }
    fseek(html, 0, SEEK_END);
    long taille = ftell(html);
    // Retourner au début du fichier
    fseek(html, 0, SEEK_SET);

    fprintf(dialogue, "HTTP/1.1 %d\r\n", code);
    fprintf(dialogue, "Content-type: %s\r\n", "text/html");
    fprintf(dialogue, "Content-length: %ld\r\n", taille);
    fprintf(dialogue, "\r\n");

    char c;
    while ((c = fgetc(html)) != EOF) {
        fputc(c, dialogue);
    }

    fclose(html);
    return 0;
}

int gestionhttp(FILE* dialogue)
{
    char requete[MAX_BUFFER];
    char cmd[MAX_BUFFER];
    char page[MAX_BUFFER];
    char proto[MAX_BUFFER];
    char reponse[MAX_BUFFER];
    while (1) {
        // Récuperation de la requete
        if (fgets(requete, MAX_BUFFER, dialogue) == NULL) {
            printf("Erreur lors de la récuperation de la requete\n");
            fprintf(dialogue, "HTTP/1.1 %d\r\n\r\n", CODE_NOTFOUND);
            return -1;
        }

        // Decoupage de la requete en 3 parties
        if (sscanf(requete, "%s %s %s", cmd, page, proto) != 3) {
            printf("Erreur lors de la conversion de la requete\n");
            fprintf(dialogue, "HTTP/1.1 %d\r\n\r\n", CODE_NOTOK);

            return -1;
        }
        while (fgets(requete, MAX_BUFFER, dialogue) != NULL) {
            if (strcmp(requete, "\r\n") == 0) {
                break;
            }
        }
        if (strcasecmp(cmd, "GET") == 0) {
            printf("Requete GET reçue : %s %s\n", page, proto);
            if (strcasecmp(page, "/") == 0) {
                strcpy(page, PAGE_HOME);
            }
            // Si reponse
            if (strlen(page) >= strlen(EXTENTION) && strcmp(page + strlen(page) - strlen(EXTENTION), EXTENTION) != 0) {
                strcpy(reponse, page);
                strcpy(page, PAGE_REPONSE);
                char* position_point_interrogation = strchr(reponse, '?');

                // Vérification de la position du point d'interrogation
                if (position_point_interrogation != NULL) {
                    // Pointeur vers la sous-chaîne après le point d'interrogation
                    char* sous_chaine = position_point_interrogation + 1;

                    // Remplacement des '=' par ';' et des '&' par ';'
                    for (int i = 0; sous_chaine[i] != '\0'; i++) {
                        if (sous_chaine[i] == '=' || sous_chaine[i] == '&') {
                            sous_chaine[i] = ';';
                        }
                    }
                    strcat(sous_chaine, "\n");
                    const char* chemin_fichier = FICHIER_CSV;

                    // Ouvrir le fichier en mode ajout ("a+"), le créer s'il n'existe pas
                    FILE* fichier = fopen(chemin_fichier, "a+");

                    // Vérifier si l'ouverture du fichier a réussi
                    if (fichier != NULL) {
                        // Ajouter la chaîne au fichier
                        fprintf(fichier, "%s", sous_chaine);

                        // Fermer le fichier
                        fclose(fichier);

                        printf("La chaîne a été ajoutée avec succès.\n");
                    } else {
                        printf("Erreur lors de l'ouverture du fichier.\n");
                    }
                }
            }

            if (envoyer_html(dialogue, page) == -1) {
                printf("Fichier %s introuvable\n", page);
                return -1;
            }

            else {
                printf("Fichier %s envoyé\n", page);
                return 0;
            }
        } else {
            printf("Méthode autre que GET, méthode non reconnue : %s\n", requete);
            return -1;
        }
    }
}
