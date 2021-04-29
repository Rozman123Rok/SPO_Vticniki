#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include<ctype.h>
#include <dirent.h>

/// Rok Rozman
// deluje samo za posiljanje datotek

int isDirectory(const char *path) {
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

int main(int argc, char *argv[])
{
    // argv[0] ./client
    // argv[1] ime dat
    // argv[2] 127.0.0.1
    // argv[3] port
    int sockfd, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;



    char buffer[512];
    if (argc < 3){
        printf("Error premalo argumentov!\n");
        return 0;
    } 
    port = atoi(argv[3]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        printf("Error odpiranje socketa!\n");
        return 0;
    } 
    server = gethostbyname(argv[2]); // nas server
    if (server == NULL) {
        printf("Error ni takega hosta!\n");
        return 0;
    } 
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        printf("Error pri povezavi!\n");
        return 0;
    } 
  
    bzero(buffer,512); // pocisti 
        
    FILE *f;
    
    int besede = 0;
    char c;
    // mores dodat ime svoje datoteke
    size_t len = strlen(argv[1]);
    char * ime_dat = malloc(len+1);
    if(ime_dat==NULL)
    {
        printf("Napaka!\n");
        return 0;
    }
    strcpy(ime_dat, argv[1]);
    printf("ime dat %s\n", ime_dat);


    /// ugotovimo ce je directory al ne
    if(isDirectory(ime_dat)){
        printf("je dir\n");
        DIR *dir;
        dir = opendir(ime_dat);
        
    }
    else{
        printf("ni dir\n");
    }

    f=fopen(ime_dat,"r"); // datoteka ki jo zelimo posiljati
    while((c=getc(f))!=EOF) // dokler nismo end of file
	{	
		fscanf(f , "%s" , buffer);
		if(isspace(c)||c=='\t'||c=='\n')
		besede++;	
	}
    besede++; // ker je drugace ena premalo
	printf("besede = %d \n"  , besede); // koliko besed je v datoteki
    printf("%s, %d\n", argv[1], sizeof(argv[1]));
    int dolzina_imena = sizeof(argv[1]);
    write(sockfd, &dolzina_imena, sizeof(int)); // pisemo na socket koliko je besed
    write(sockfd, ime_dat, dolzina_imena); // poslemo ime datoteke
	write(sockfd, &besede, sizeof(int)); // pisemo na socket koliko je besed
    rewind(f); // da pointer na zacetek
 
    char ch ;
       while(ch != EOF)
      {
		
		fscanf(f , "%s" , buffer); // beremo iz datoteke
		printf("%s\n" , buffer);
		write(sockfd,buffer,512); // pisemo dejanske podatke
		ch = fgetc(f);
      }
	printf("Datoteka uspesno poslana!\n");
    free(ime_dat);
    close(sockfd); // zapremo socket
    return 0;
}