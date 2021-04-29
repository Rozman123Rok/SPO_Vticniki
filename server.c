#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

/// Rok Rozman
// deluje samo za posiljanje datotek

int main(int argc, char *argv[]) {
    int sockfd, newsockfd, port;             // socketi oz socket file desc
    socklen_t client_dolzina;                // dolzina client naslova
    char buffer[512];                        // buffer za branje
    struct sockaddr_in server_addr, client_addr;  // naslova za server in client
    int n;
    if (argc < 2) {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("Error pri odpiranju socket!\n");
        return 0;
    }
    bzero((char *)&server_addr, sizeof(server_addr)); // pocistimo server addr

    port = atoi(argv[1]); // dobimo port iz argumentov
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // localhost
    server_addr.sin_port = htons(port); // dolocimo port
    // naredimo povezavo
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Error pri povezovanju!\n");
        return 0;
    };
    listen(sockfd, 5); // poslusamo na socket
    client_dolzina = sizeof(client_addr); // dobimo dolzino naslova
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_dolzina); // sprejmemo novi socket
    if (newsockfd < 0) {
        printf("Error sprejem!\n");
        return 0;
    }

    FILE *fp; // ustvarimo pointer
    int ch = 0;
    // se moremo nekak spravit ime dat
    int dolzina_imena;
    read(newsockfd, &dolzina_imena, sizeof(int)); // preberemo stevilo crk
    printf("dolzina imena: %d\n", dolzina_imena);
    //char test[dolzina_imena];

    char * test = malloc(dolzina_imena+1);
    if(test==NULL)
    {
        printf("Napaka!\n");
        return 0;
    }

    printf("test :%d\n", sizeof(test));
    read(newsockfd, test, dolzina_imena);   // beremo iz socketa
    printf("%s\n", test);
    fp = fopen(test, "a"); // odpremo datoteko
    //fp = fopen("prejeto.txt", "a");
    int besede; // stevilo besed
    read(newsockfd, &besede, sizeof(int)); // preberemo stevilo besed
    printf("Prejeto stevilo besed : %d\n", besede);  // koliko besed smo dobili
    while (ch != besede) {
        read(newsockfd, buffer, 512);   // beremo iz socketa
        fprintf(fp, " %s", buffer);     // pisemo v datoteko
        printf(" %s %d ", buffer, ch);  // kaj smo brali oz pisali
        ch++;
    }
    printf("Datoteka uspesno prejeta\n");
    close(newsockfd);
    close(sockfd);
    free(test);
    return 0;
}

