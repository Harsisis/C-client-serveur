#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define true 1
#define false 0
#define SERVER_BACKLOG 100 //number of connections at the same time

//ticket structure
typedef struct ticket
{
    char *uuid;
    char *familyClient;
    char *nameClient;
    int available;
} ticket ;

//create the table of 100 tickets
ticket table[100];
int i = 0;
void * handle_connection(void* p_client_socket);


int main () {

    //filling table --> uuid =-1 | char = NULL | available = 0
    for (int j = 0; j < 100; j++){
        table[j] = (struct ticket) {"-1", NULL, NULL, 0};
    }

    int server_socket, client_socket, addr_size;

    //create the server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    //bind the socket to our IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket, SERVER_BACKLOG);

    //srand
    srand(time(NULL));

    while ((client_socket = accept(server_socket, NULL, NULL))) {

        //client is accepted now we do whatever we want with connections
        pthread_t t;
        pthread_create(&t, NULL, handle_connection, &client_socket);
        i++;
    }
    close(server_socket);
    return 0;
}

void * handle_connection(void* p_client_socket) {

    int client_socket = *((int*)p_client_socket);
    char client_message[256];
    char *uuid;
    char *family;
    int c;

    int idClient = i;
    printf("Client %d connected\n", idClient);
    //read the client choice
    read(client_socket, client_message, 256);
    printf("Client %d message : %s\n", idClient, client_message);
    if (!strcmp(client_message, "book")){
        //check if a ticket is available
        int available = false;
        int j = 0;
        while (!available && j < 100) {
            if (table[j].available == 0) {
                //change available to true
                table[j].available = 1;
                //a ticket is available
                available = true;
            } else {
                j++;
            }
        }
        if (j >= 100){
            write(client_socket, "Aucune place n'est disponible", 64);
            i--;
        } else if (available){
            write(client_socket, "Au moins une place est disponible\n", 64);
            //ask client name and family name
            //wait for the family
            read(client_socket, client_message, 256);
            table[j].familyClient = strdup(client_message);
            //wait for the name
            read(client_socket, client_message, 256);
            table[j].nameClient = strdup(client_message);
            //set the new uuid
            c = 0;
            uuid = (char *) malloc(sizeof(char) * 11);
            srand(time(NULL));
            while (c < 10){
                uuid[c] = '0' + (rand() % 10);
                c++;
            }
            uuid[10] = '\0';
            table[j].uuid = strdup(uuid);
            //write to the client his uuid
            printf("Client %d uuid : %s\n", i, table[j].uuid);
            write(client_socket, table[j].uuid, 12);

            i--;
        }
    } else if (!strcmp(client_message, "cancel")){

        read(client_socket, client_message, 256);
        uuid = strdup(client_message);

        read(client_socket, client_message, 256);
        family = strdup(client_message);

        printf("searching : uuid = %s | family name = %s\n", uuid, family);

        int j = 0;
        while ( j < 100) {
            //print table
            //printf("%i\t%s\t%s\t%s\n", j, table[j].uuid, table[j].familyClient, table[j].nameClient);
            if (table[j].available == 1 && !strcmp(table[j].uuid, uuid) && !strcmp(table[j].familyClient, family)) {
                //ticket found
                printf("ticket found\n");
                table[j].familyClient = NULL;
                table[j].nameClient = NULL;
                table[j].uuid = "-1";
                table[j].available = 0;
                write(client_socket, "Place annulée avec succès !\0", 29);

                i--;
                j = 101;
            } else {
                j++;
            }
        }

        if (j == 100){
            write(client_socket, "Aucune place enregistrée ne correspond à la demande", 64);
            i--;
        }
    }
    close(client_socket);
}