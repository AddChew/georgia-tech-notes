#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define BUFSIZE 1024
#define SOCKET_ERROR -1
#define USAGE                                                        \
    "usage:\n"                                                         \
    "  echoserver [options]\n"                                         \
    "options:\n"                                                       \
    "  -m                  Maximum pending connections (default: 5)\n" \
    "  -p                  Port (Default: 39483)\n"                    \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"help",          no_argument,            NULL,           'h'},
    {"maxnpending",   required_argument,      NULL,           'm'},
    {"port",          required_argument,      NULL,           'p'},
    {NULL,            0,                      NULL,             0}
};


int main(int argc, char **argv) {
    int i;
    int reuse = 1;
    int portno = 39483; /* port to listen on */
    int option_char;

    int maxnpending = 5;
    int status, newfd, sockfd, bytes_received, total_bytes_received;
    
    char buffer[BUFSIZE + 1];
    char *portno_str = "65535";
    
    struct addrinfo hints, *res, *p;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof client_addr;
  
    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "p:m:hx", gLongOptions, NULL)) != -1) {
        switch (option_char) {
        case 'h': // help
            fprintf(stdout, "%s ", USAGE);
            exit(0);
            break;
        case 'p': // listen-port
            portno_str = optarg;
            portno = atoi(optarg);
            break;                                        
        case 'm': // server
            maxnpending = atoi(optarg);
            break; 
        default:
            fprintf(stderr, "%s ", USAGE);
            exit(1);
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }
    if (maxnpending < 1) {
        fprintf(stderr, "%s @ %d: invalid pending count (%d)\n", __FILE__, __LINE__, maxnpending);
        exit(1);
    }

    // Socket code
    memset(&hints, 0, sizeof hints);
    memset(&buffer, '\0', sizeof buffer);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, portno_str, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return status;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == SOCKET_ERROR) {
            perror("socket error");
            continue;
        };

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse) == SOCKET_ERROR) {
            perror("setsockopt error");
            continue;  
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof reuse) == SOCKET_ERROR) {
            perror("setsockopt error");
            continue; 
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
            perror("bind error");
            close(sockfd);
            continue;        
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "%s @ %d: server failed to bind to %d\n", __FILE__, __LINE__, portno);
        exit(1);
    }

    if (listen(sockfd, maxnpending) == SOCKET_ERROR) {
        perror("listen error");
        return errno;        
    }

    newfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_size);
    if (newfd == SOCKET_ERROR) {
        perror("accept error");
        return errno;
    }

    while ((bytes_received = recv(newfd, buffer, BUFSIZE, 0)) > 0) {
        printf("%s", buffer);
        for (i = 0; i < strlen(buffer); i++) {
            buffer[i] = toupper(buffer[i]);
        }
        if (send(newfd, buffer, bytes_received, 0) == SOCKET_ERROR) {
            perror("send error");
            return errno;
        }
        memset(&buffer, '\0', bytes_received);
    }

    if (bytes_received == SOCKET_ERROR) {
        perror("recv error");
        return errno;
    }
    close(newfd);
    close(sockfd);
    freeaddrinfo(res);

    return 0;
}