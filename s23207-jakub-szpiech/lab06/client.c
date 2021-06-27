#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(char *msg) /* komuniakt o bledzie stderr i przerwanie programu */
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;

    struct sockaddr_in serv_addr; /* zawiera adres IP serwera */
    struct hostent *server;

    char buffer[256]; /* reads characters from connection and saves it in this buffer */
    if (argc < 3) { /* tu trzeba podac numer portu, po ktorym laczymy sie z sserwerem */
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]); /* numer portu */
    sockfd = socket(AF_INET, SOCK_STREAM, 0); /* tworzy gniazdo, pierwszy argument to adres gniazda, drugi to typ gniazda, trzeci to protokol */
    if (sockfd < 0)  /* informacja zwrotna w przypadku niepowodzenia operacji */
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) { /* arg[1] przechowuj nazwe hosta */
        fprintf(stderr,"ERROR, no such host\n"); /* informacja zwrotna w przypadku niepowodzenia wywolanai gniazda */
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); /* ustawia wszystkie wartosci w bufiorze na 0*/
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno); /* sin_port przechowuje numer portu, fonkcja htons przerabia port  na numer portu w sieciowej kolejnosci bajtow */ 
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)  /* wywolywane przez klienta w celu polaczenia sie z serwere. Potrzebuje 3 argumentow, deskryptor,
										adres hosta i rozmiar hosta */
        error("ERROR connecting");
    printf("Please enter the message: "); /* input uzytkownika */
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    return 0;
}
