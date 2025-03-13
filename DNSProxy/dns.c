#include "dns.h"

int to_forward(char *buffer, int buffer_len, char *response, int *response_len) {
    int dns_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (dns_sock < 0) {
        perror("Erreur de création du socket");
        return 0;
    }

    struct sockaddr_in dns_server;
    dns_server.sin_family = AF_INET;
    dns_server.sin_port = htons(53);
    inet_pton(AF_INET, "8.8.8.8", &dns_server.sin_addr);

    // Envoyer la requête au serveur DNS distant
    if (sendto(dns_sock, buffer, buffer_len, 0, (struct sockaddr *)&dns_server, sizeof(dns_server)) < 0) {
        perror("Erreur d'envoi au serveur DNS distant");
        close(dns_sock);
        return 0;
    }

    // Réception de la réponse du serveur DNS distant
    socklen_t server_len = sizeof(dns_server);
    int recv_len = recvfrom(dns_sock, response, 512, 0, (struct sockaddr *)&dns_server, &server_len);

    close(dns_sock);

    if (recv_len > 0) {
        *response_len = recv_len;
        return 1;
    } else {
        perror("Erreur de réception depuis le serveur DNS distant");
        return 0;
    }
}

void querry_process(int client_socket, struct sockaddr_in *client_addr, socklen_t *length) {
    uint8_t buffer[512];
    uint8_t response[512];
    int recv_len, resp_len;

    // Recevoir la requête du client (dig)
    recv_len = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)client_addr, length);
    if (recv_len < 0) {
        perror("Erreur de réception de la requête DNS");
        return;
    }

    // Convertir l'adresse du client en string pour l'afficher
    char client[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr->sin_addr, client, INET_ADDRSTRLEN);
    //printf("Requête reçue de %s\n", client);

    // Désérialiser le message (si nécessaire pour traitement)
    dns_message_t message;
    deserialize_message(buffer, recv_len, &message);

    // Vérifier si c'est pour "wikipedia"
    int is_wikipedia = 0;
    for (int i = 0; i < message.header.qd_count; i++) {
        if (strstr(message.question[i].q_name, "wikipedia") != NULL) {
            is_wikipedia = 1;
            break;
        }
    }

    if (is_wikipedia) {
        printf("C'est à moi : réponse locale pour Wikipedia.\n");
        // TODO : Générer une réponse DNS locale et l’envoyer au client
    } else {
        printf("Forwarding DNS request...\n");

        // Forward vers le DNS distant et recevoir une réponse
        if (to_forward((char *)buffer, recv_len, (char *)response, &resp_len)) {
            printf("Réponse reçue, renvoi au client %s\n", client);

            // Envoyer la réponse au client (dig)
            if (sendto(client_socket, response, resp_len, 0, (struct sockaddr *)client_addr, *length) < 0) {
                perror("Erreur d'envoi de la réponse au client");
            }
        }
    }
}

void deserialize_message(uint8_t *buffer, size_t len, dns_message_t *message) {
    size_t offset = 0;
    uint16_t temp;

    // Désérialisation du header
    memcpy(&temp, buffer + offset, sizeof(temp));
    message->header.id = ntohs(temp);
    offset += sizeof(temp);

    memcpy(&temp, buffer + offset, sizeof(temp));
    message->header.flags = ntohs(temp);
    offset += sizeof(temp);

    memcpy(&temp, buffer + offset, sizeof(temp));
    message->header.qd_count = ntohs(temp);
    offset += sizeof(temp);

    memcpy(&temp, buffer + offset, sizeof(temp));
    message->header.an_count = ntohs(temp);
    offset += sizeof(temp);

    memcpy(&temp, buffer + offset, sizeof(temp));
    message->header.ns_count = ntohs(temp);
    offset += sizeof(temp);

    memcpy(&temp, buffer + offset, sizeof(temp));
    message->header.ar_count = ntohs(temp);
    offset += sizeof(temp);

    // Désérialisation des questions
    for (int i = 0; i < message->header.qd_count; i++) {
        offset += decode_dns_name(buffer, offset, message->question[i].q_name);

        memcpy(&temp, buffer + offset, sizeof(temp));
        message->question[i].q_type = ntohs(temp);
        offset += sizeof(temp);

        memcpy(&temp, buffer + offset, sizeof(temp));
        message->question[i].q_class = ntohs(temp);
        offset += sizeof(temp);
    }

    // Désérialisation des sections Answer, Authority et Additional
    dns_record_t *records[] = { message->answer, message->authority, message->additional };
    uint16_t record_counts[] = {
        message->header.an_count,
        message->header.ns_count,
        message->header.ar_count
    };

    for (int section = 0; section < 3; section++) {
        for (int i = 0; i < record_counts[section]; i++) {
            offset += decode_dns_name(buffer, offset, records[section][i].name);

            memcpy(&temp, buffer + offset, sizeof(temp));
            records[section][i].type = ntohs(temp);
            offset += sizeof(temp);

            memcpy(&temp, buffer + offset, sizeof(temp));
            records[section][i].class = ntohs(temp);
            offset += sizeof(temp);

            uint32_t ttl;
            memcpy(&ttl, buffer + offset, sizeof(ttl));
            records[section][i].ttl = ntohl(ttl);
            offset += sizeof(ttl);

            memcpy(&temp, buffer + offset, sizeof(temp));
            records[section][i].rdlength = ntohs(temp);
            offset += sizeof(temp);

            memcpy(records[section][i].rdata, buffer + offset, records[section][i].rdlength);
            offset += records[section][i].rdlength;
        }
    }
}

size_t decode_dns_name(uint8_t *buffer, size_t offset, char *output) {
    size_t original_offset = offset;
    size_t out_offset = 0;
    uint8_t length;

    while ((length = buffer[offset]) > 0) {
        if ((length & 0xC0) == 0xC0) { // Compression (pointeur)
            uint16_t pointer = ((length & 0x3F) << 8) | buffer[offset + 1];
            offset = pointer; // Aller à l’adresse pointée
        } else { // Lecture normale du label
            offset++;
            memcpy(output + out_offset, buffer + offset, length);
            out_offset += length;
            output[out_offset++] = '.'; // Ajouter un point
            offset += length;
        }
    }

    if (out_offset > 0) {
        output[out_offset - 1] = '\0'; // Supprimer le dernier "."
    } else {
        output[0] = '\0';
    }

    return (offset - original_offset) + 1;
}
