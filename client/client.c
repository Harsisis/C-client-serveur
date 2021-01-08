#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>


int main () {

    //create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //specify an address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
    //check if connection is ok
    if (connection_status == -1){
        printf("ERROR in connection to the remote socket\n\n");
    }

    char server_message[256];
    char client_message[256];

    //print out the server's data response
    printf("Bienvenue sur le serveur ! :D\n");

    //client choice book or cancel reservation
    char *choice;
    printf("Souhaitez vous réserver (r) ou annuler (a) ? \n");
    scanf("%s", choice);
    if (!strcmp(choice, "r")){
        printf("mode choisi : Réservation \n");
        write(server_socket, "book", 8);
        //at least one place is available or everything is full
        read(server_socket, server_message, sizeof(server_message));
        printf("%s\n", server_message);
        //at least one place
        if (!strcmp(server_message, "Au moins une place est disponible\n")){
            printf("Veuillez saisir votre nom\n");
            scanf("%s", choice);
            write(server_socket, choice, sizeof(choice));

            printf("Veuillez saisir votre prénom\n");
            scanf("%s", choice);
            write(server_socket, choice, sizeof(choice));

            //print uuid
            printf("%s\n", server_message);
            read(server_socket, server_message, sizeof(server_message));
            printf("Votre numéro de dossier est : %s\n", server_message);
        }
    } else if (!strcmp(choice, "a")) {
        printf("mode choisi : Annulation \n");
        write(server_socket, "cancel", 16);

        //input uuid and name
        printf("veuillez saisir le numéro de votre dossier :\n");
        scanf("%s", client_message);
        write(server_socket, client_message, sizeof(client_message));

        printf("veuillez saisir votre nom :\n");
        scanf("%s", client_message);
        write(server_socket, client_message, sizeof(client_message));

        //success or place not found
        read(server_socket, server_message, sizeof(server_message));
        printf("%s\n", server_message);
    }
    close(server_socket);
    return 0;
}
