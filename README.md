Pour lancer le serveur, vous pouvez exécuter la commande ```make && make start_sioux``` pour démarrer le serveur directement sur le port 8080.
Pour vous y connecter, veuillez ouvrir Firefox et entrer l'adresse suivante : ```localhost:8080```.
Cela vous permettra d'accéder directement à nos pages pour voter pour votre image favorite.
Les résultats seront stockés dans un fichier .csv qui se trouve dans le répertoire sioux.

# Serveur Ablette

Pour lancer Ablette, vous pouvez exécuter la commande ```make && make start``` à la racine du projet. Le serveur affichera les différents paquets TCP et les triera par ordre décroissant en fonction des adresses IP qui communiquent le plus avec votre cible. Le classement s'affiche tous les 5 paquets reçus mais peut être modifié en ajustant le code dans ablette.c (Frequence_aff). Les paquets sont triés par les ports 80 (http) et 443 (https).

# Nettoyage

Vous pouvez utiliser la commande ```make clean``` pour nettoyer tous les répertoires du projet.
