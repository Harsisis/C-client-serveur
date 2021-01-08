# Explications des choix :

Nous avons fait le choix, pour ce projet, de fonctionner avec des threads.

Les, 100, places de concert sont stockées dans une table de 100 cases avec dans chacunes des cases,
une structure.
La structure ticket comprend différent paramètres dont, l'uuid de l'utilisateur, son nom, son prénom ainsi qu'un
"booléen" qui indique si la place est disponible ou non.

Par défaut au lancement du serveur, toutes les places sont vides et donc le booléen est à 0.

Lorsqu'un utilisateur souhaite réserver, le serveur parcours la table des places jusqu'à trouver une place disponible,
si il n'y en a pas, l'utilisateur est prévenu puis déconnecté, dans le cas contraire, l'utilisateur est aussi prévenu et est invité à saisir ses informations.
Ensuite le serveur envoie au client un uuid de 10 nombres, généré aléatoirement avec la librairie time.h .

Lorsqu'un utilisateur souhaite annuler, le serveur lui demande son nom ainsi que son numéro de ticket (uuid précédemment donné), puis parcours la table des tickets.
Si le serveur ne trouve aucune correspondance alors l'utilisateur est prévenu et est déconnecté, mais dans le cas ou le serveur trouve une correspondance,
il efface les données saisies par l'utilisateur et remet les données par défaut et prévient l'utilisateur que sa place est bien annulée.


# Compilation :

Aucun argument n'est demandé au démarrage des applications, les commandes de compilation sont donc :

gcc client.c -o client

gcc serv.c -o serv

A exécuter dans les bon dossiers
