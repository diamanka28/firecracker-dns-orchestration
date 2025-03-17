//RESSOURCE RFC: https://datatracker.ietf.org/doc/html/rfc1035

// INCLUDING
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

// LES DEFINITIONS
#define DNS_PORT 2025
#define MAX_DN_LENGTH 255
#define MAX_QUESTION 20
#define MAX_RECORD 20
#define DOMAIN "psar_firecracker"

// DEFINITION STRUCTURES
typedef struct header
{
    uint16_t id;    // identifiant unique de la requete
    uint16_t flags;     // QR, OPCODE, AA, TC, RD, RA, Z, RCODE
    uint16_t qd_count;  //nombre de question
    uint16_t an_count;  //nombre de réponse
    uint16_t ns_count;  // nombre d'entrée authority
    uint16_t ar_count;  // nombre d'entrée additional

} dns_header_t;

typedef struct question
{
    char q_name[MAX_DN_LENGTH]; //le nom du domaine recherché
    uint16_t q_type;    // type de la requette (A , NS , SNAME, ...)
    uint16_t q_class;   // la classe (IN pour Internet)

} dns_question_t;

typedef struct reponse
{
    char name[MAX_DN_LENGTH];
    uint16_t type;
    uint16_t class;
    uint32_t ttl;
    uint16_t rdlength;
    uint8_t rdata[256];

} dns_record_t;

typedef struct dns
{
    dns_header_t header;
    dns_question_t question[MAX_QUESTION];
    dns_record_t answer[MAX_RECORD];
    dns_record_t authority[MAX_RECORD];
    dns_record_t additional[MAX_RECORD];

} dns_message_t;

// DECLARATION DES FGONCTIONS
void querry_process(int client_socket, struct sockaddr_in *client_addr, socklen_t *length);
int to_forward(char* buffer, int taille, char * response, int * response_len);
void deserialize_message(uint8_t * buffer, size_t taille, dns_message_t *message);
size_t decode_dns_name(uint8_t* buffer, size_t offset, char* output);
void find_and_launch_service(const char *fonction);

// Liste qui stocke les fonctions disponibles du serveur 

// Déclaration des services
typedef int (*service_func_t)();
typedef struct {
    const char *name;      // Nom de la fonction
    service_func_t func;   // Pointeur vers la fonction
} service_t;

extern service_t services[]; // Liste globale des services
extern const int num_services; // Nombre de services dans la liste

// Déclaration des fonctions de service
int func_A();
int func_B();

