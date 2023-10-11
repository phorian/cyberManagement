#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *str)
{
    perror(str);
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "Port No not provided.\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[255];

    struct sockaddr_in server_addr, cli_addr;
    socklen_t clilen;

    sockfd= socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        error("Error opening Sockets.");
    }

    bzero((char *) &server_addr, sizeof(server_addr));
    portno = atoi(argv[1]);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *) &server_addr,  sizeof(server_addr))< 0)
        error("Binding failed\n");
    
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if(newsockfd < 0 )
    error("Not Accepted\n");

    while (1)
    {
        bzero(buffer, 255);
        n = read(newsockfd, buffer, 255);
        if(n < 0)
            error("Failed to read\n");
        printf("client : %s\n", buffer);
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);

        n = write(newsockfd, buffer, strlen(buffer));
        if(n < 0)
            error("Failed to Write.\n");

        int i = strcmp("Bye", buffer, 3);
        if(i == 0)
            break;
    }

    close(newsockfd);
    close(sockfd);
    return 0;
    
}