#include "dns_utils.h"

int main(int argc, char const *argv[]) {
    printf("nom du domaine : %s \n ",DOMAIN);
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Création du socket UDP
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Erreur lors de la création du socket");
        return EXIT_FAILURE;
    }

    // Configuration du serveur
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(DNS_PORT);

    // Liaison du socket au port 53
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erreur lors de la liaison du socket");
        close(server_socket);
        return EXIT_FAILURE;
    }

    printf("Serveur DNS en écoute sur le port %d...\n", DNS_PORT);

    // Boucle infinie pour écouter les requêtes
    while (1) {
        querry_process(server_socket, &client_addr, &client_len);
    }

    // Ne devrait jamais arriver
    close(server_socket);
    return 0;
}

//l'aspect réseau et orchestration
