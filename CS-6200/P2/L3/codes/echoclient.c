#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <getopt.h>

/* Be prepared accept a response of this length */
#define BUFSIZE 1024
#define SOCKET_ERROR -1
#define USAGE                                                                       \
    "usage:\n"                                                                      \
    "  echoclient [options]\n"                                                      \
    "options:\n"                                                                    \
    "  -s                  Server (Default: localhost)\n"                           \
    "  -m                  Message to send to server (Default: \"Hello Spring!!\")\n" \
    "  -p                  Port (Default: 39483)\n"                                  \
    "  -h                  Show this help message\n"

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
    {"port", required_argument, NULL, 'p'},
    {"server", required_argument, NULL, 's'},
    {"help", no_argument, NULL, 'h'},
    {"message", required_argument, NULL, 'm'},
    {NULL, 0, NULL, 0}};

/* Main ========================================================= */
int main(int argc, char **argv)
{
    int option_char = 0;
    int status, sockfd, bytes_received;

    struct addrinfo hints, *res, *p;
    unsigned short portno = 39483;
    char *hostname = "localhost";
    char *message = "Hello Fall!!";
    char *portno_str = "65535";
    char response[BUFSIZE + 1];

    // Parse and set command line arguments
    while ((option_char = getopt_long(argc, argv, "s:p:m:hx", gLongOptions, NULL)) != -1) {
        switch (option_char) {
        case 'p': // listen-port
            portno_str = optarg;
            portno = atoi(optarg);
            break;
        case 'm': // message
            message = optarg;
            break;
        case 's': // server
            hostname = optarg;
            break;
        case 'h': // help
            fprintf(stdout, "%s", USAGE);
            exit(0);
            break;
        default:
            fprintf(stderr, "%s", USAGE);
            exit(1);
        }
    }

    setbuf(stdout, NULL); // disable buffering

    if ((portno < 1025) || (portno > 65535)) {
        fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
        exit(1);
    }

    if (NULL == message) {
        fprintf(stderr, "%s @ %d: invalid message\n", __FILE__, __LINE__);
        exit(1);
    }

    if (NULL == hostname) {
        fprintf(stderr, "%s @ %d: invalid host name\n", __FILE__, __LINE__);
        exit(1);
    }

    // Socket Code
    memset(&hints, 0, sizeof hints);
    memset(&response, '\0', sizeof response);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, portno_str, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return status;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == SOCKET_ERROR) {
            perror("socket error");
            continue;
        };

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == SOCKET_ERROR) {
            perror("connect error");
            close(sockfd);
            continue;        
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "%s @ %d: client failed to connect to %s:%d\n", __FILE__, __LINE__, hostname, portno);
        exit(1);
    }

    int bytes_send;
    int total_bytes_send = 0;
    int messagelen = strlen(message);

    while ((bytes_send = send(sockfd, message, messagelen, 0)) > 0) {
        if ((recv(sockfd, response, BUFSIZE, 0)) == SOCKET_ERROR) {
            perror("recv error");
            return errno;
        }
        printf("%s", response);
        memset(&response, '\0', sizeof response);
        total_bytes_send += bytes_send;
        if (total_bytes_send >= messagelen) {
            break;
        }
    }

    if (bytes_send == SOCKET_ERROR) {
        perror("send error");
        return errno;
    }

    close(sockfd);
    freeaddrinfo(res);

    return 0;
}