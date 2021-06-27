/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)  /* komuniakt o bledzie stderr i przerwanie programu */
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno, clilen; 
     char buffer[256]; /* reads characters from connection and saves it in this buffer */
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) { /* tu trzeba podac numer portu, po ktorym laczymy sie z sserwerem */
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0); /* tworzy gniazdo, pierwszy argument to adres gniazda, drugi to typ gniazda, trzeci to protokol */
     if (sockfd < 0)  /* informacja zwrotna w przypadku niepowodzenia operacji */
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr)); /* ustawia wszystkie wartosci wbuforze na 0  */
     portno = atoi(argv[1]); /* numer portu */
     serv_addr.sin_family = AF_INET; /* zmienna serv_addr jest typu sockaddr_in, z 4 polami */
     serv_addr.sin_addr.s_addr = INADDR_ANY; /* przechowuje adres IP hosta */
     serv_addr.sin_port = htons(portno);/* sin_port przechowuje numer portu, fonkcja htons przerabia port  na numer portu w sieciowej kolejnosci bajtow */
     if (bind(sockfd, (struct sockaddr *) &serv_addr, /* wywolywane przez klienta w celu polaczenia sie z serwere. Potrzebuje 3 argumentow, deskryptor,
										adres hosta i rozmiar hosta */
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); /*  accept() blokuje proces, dopoki kilent nie polaczy sie z serwerem */
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256); /* inicjuje bufor, odczytuje wartosc z gniazda, wywolanie read uzywa nowego deskryptora pliku */
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer); /* print widoczny dla klienta  i hosta */
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     return 0; 
}

