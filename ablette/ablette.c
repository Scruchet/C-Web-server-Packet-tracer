#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define Frequence_aff 5

struct AdresseStatistique
{
    char adresseIP[INET_ADDRSTRLEN];
    int nombrePaquets;
};

void TraiterPaquet(unsigned char *, int);
void ImprimerPaquetTCP(unsigned char *, int);
void AfficherStatistiques();
void TrierStatistiques();

int sock_brut;
struct AdresseStatistique statistiques[100];
int nombreAdresses = 0;
int total = 0;

int main()
{
    int taille_saddr, taille_donnees;
    struct sockaddr saddr;
    unsigned char *buffer = (unsigned char *)malloc(65536); // C'est grand !
    printf("Démarrage...\n");

    // Créer un socket brut pour l'écoute
    sock_brut = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock_brut < 0)
    {
        perror("Erreur de socket");
        return 1;
    }

    while (1)
    {
        taille_saddr = sizeof saddr;
        // Recevoir un paquet
        taille_donnees = recvfrom(sock_brut, buffer, 65536, 0, &saddr, &taille_saddr);
        if (taille_donnees < 0)
        {
            perror("Erreur de recvfrom");
            return 1;
        }
        // Traiter le paquet
        TraiterPaquet(buffer, taille_donnees);
    }

    close(sock_brut);
    printf("Terminé");
    return 0;
}

void TraiterPaquet(unsigned char *buffer, int taille)
{
    // Obtenir la partie d'en-tête IP de ce paquet
    struct iphdr *iph = (struct iphdr *)buffer;

    // Filtrer uniquement les paquets pour les ports 80 (HTTP) et 443 (HTTPS)
    struct tcphdr *tcph = (struct tcphdr *)(buffer + iph->ihl * 4);
    if (iph->protocol == IPPROTO_TCP && (ntohs(tcph->source) == 80 || ntohs(tcph->source) == 443 || ntohs(tcph->dest) == 80 || ntohs(tcph->dest) == 443))
    {
        // Le paquet correspond aux ports 80 (HTTP) ou 443 (HTTPS)

        // Convertir les adresses IP source en chaînes
        char sourceIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(iph->saddr), sourceIP, INET_ADDRSTRLEN);

        // Trouver l'adresse IP source dans le tableau de statistiques
        int indiceAdresse = -1;
        for (int i = 0; i < nombreAdresses; i++)
        {
            if (strcmp(statistiques[i].adresseIP, sourceIP) == 0)
            {
                indiceAdresse = i;
                break;
            }
        }

        // Si l'adresse IP n'est pas dans le tableau, l'ajouter
        if (indiceAdresse == -1)
        {
            // Vérifier si le tableau est plein
            if (nombreAdresses < 100)
            {
                // Ajouter l'adresse IP au tableau de statistiques
                strncpy(statistiques[nombreAdresses].adresseIP, sourceIP, INET_ADDRSTRLEN);
                statistiques[nombreAdresses].nombrePaquets = 1;
                nombreAdresses++;
            }
            else
            {
                printf("Tableau de statistiques plein. Augmentez la taille du tableau.\n");
            }
        }
        else
        {
            // Si l'adresse IP est déjà dans le tableau, incrémenter le nombre de paquets
            statistiques[indiceAdresse].nombrePaquets++;
        }

        // Le reste du code pour imprimer et traiter le paquet
        if (total % Frequence_aff == 0)
        {
            TrierStatistiques();
            AfficherStatistiques();
        }

        total++;
    }
}

void ImprimerDonnees(unsigned char *donnees, int Taille)
{
    for (int i = 0; i < Taille; i++)
    {
        if (i != 0 && i % 16 == 0)
        {
            printf("         \n");
        }

        if (i % 16 == 0)
            printf("   ");
        printf(" %02X", (unsigned int)donnees[i]);

        if (i == Taille - 1)
        {
            for (int j = 0; j < 15 - i % 16; j++)
                printf("   ");
            printf("         \n");
        }
    }
}

void ImprimerPaquetTCP(unsigned char *Buffer, int Taille)
{
    unsigned short longueur_en_tete_ip;

    struct iphdr *iph = (struct iphdr *)Buffer;
    longueur_en_tete_ip = iph->ihl * 4;

    struct tcphdr *tcph = (struct tcphdr *)(Buffer + longueur_en_tete_ip);

    printf("\n\n***********************Paquet TCP*************************\n");
    printf("\n");
    printf("En-tête TCP\n");
    printf("   |-Port source      : %u\n", ntohs(tcph->source));
    printf("   |-Port destination : %u\n", ntohs(tcph->dest));

    printf("En-tête IP\n");
    ImprimerDonnees(Buffer, longueur_en_tete_ip);

    printf("En-tête TCP\n");
    ImprimerDonnees(Buffer + longueur_en_tete_ip, tcph->doff * 4);

    printf("Charge utile de données\n");
    ImprimerDonnees(Buffer + longueur_en_tete_ip + tcph->doff * 4, (Taille - tcph->doff * 4 - iph->ihl * 4));

    printf("\n###########################################################\n\n\n");
}

void AfficherStatistiques()
{
    printf("\n\n******************** Statistiques des 5 adresses IP les plus fréquentes ********************\n");
    // Afficher uniquement les 5 premières adresses IP (si elles existent)
    int nombreAffichage = nombreAdresses < 5 ? nombreAdresses : 5;
    for (int i = 0; i < nombreAffichage; i++)
    {
        printf("Adresse IP : %s | Nombre de paquets : %d\n", statistiques[i].adresseIP, statistiques[i].nombrePaquets);
    }
    printf("**************************************************************\n\n");
}

void TrierStatistiques()
{
    for (int i = 0; i < nombreAdresses - 1; i++)
    {
        for (int j = 0; j < nombreAdresses - i
        for (int j = 0; j < nombreAdresses - i - 1; j++)
        {
            if (statistiques[j].nombrePaquets < statistiques[j + 1].nombrePaquets)
            {
                struct AdresseStatistique temp = statistiques[j];
                statistiques[j] = statistiques[j + 1];
                statistiques[j + 1] = temp;
            }
        }
    }
}
