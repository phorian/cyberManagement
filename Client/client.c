#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


/*
filename server_ipaddress portno

argv[0] filename
argv[1] server_ipaddress
argv[2] portno
 
*/


void error(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    int sockfd, portno, n;
    struct sockaddr_in server_addr;
    struct  hostent *server;
    
    char buffer[255];

    portno = atoi(argv[2]);
    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening Sockets.");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, No such host");
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(portno);

    if(connect(sockfd, (struct  sockaddr *) &server_addr, sizeof(server_addr))< 0)
        error("Connection failed");

        while (1)
        {
            bzero(buffer, 255);
            fgets(buffer, 255, stdin);
            n = write(sockfd, buffer, strlen(buffer));
            if(n < 0)
                error("Failed to write");

            bzero(buffer, 255);
            n = read(sockfd, buffer, 255);
            if(n < 0)
                error("Reading error");
            printf("Server: %s", buffer);

            int i = strcmp("Bye", buffer);
            if(i == 0)
            exit(0);
        }

        close(sockfd);
        return 0;
        

}