#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#endif

// Define the MinIO server URL and bucket name
#define MINIO_HOST "127.0.0.1"
#define MINIO_PORT "9001"
#define MINIO_BUCKET "pictures"

void send_http_get_request() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        perror("WSAStartup");
        exit(1);
    }
    SOCKET sockfd;
#else
    int sockfd;
    struct addrinfo hints, *res;
#endif

    char request[1024];

    snprintf(request, sizeof(request),
             "GET /browser/pictures/Order-Banner.jpg HTTP/1.1\r\n"
             "Host: %s:%s\r\n"
             "\r\n",
             MINIO_HOST, MINIO_PORT, MINIO_HOST, MINIO_PORT);

    printf("%s\nskituljko\n", request);

#ifdef _WIN32
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
#else
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
#endif

#ifdef _WIN32
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(MINIO_PORT));
    server_addr.sin_addr.s_addr = inet_addr(MINIO_HOST);

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        perror("connect");
        closesocket(sockfd);
        WSACleanup();
        exit(1);
    }
#else
    if (getaddrinfo(MINIO_HOST, MINIO_PORT, &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        close(sockfd);
        exit(1);
    }

    freeaddrinfo(res);
#endif

    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("send");
#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
        exit(1);
    }

    char buffer[2048];
    ssize_t bytes_received;

    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        // Print the received data to the console
        printf(buffer , bytes_received, stdout);
    }

#ifdef _WIN32
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
}

int main() {
    send_http_get_request();
    return 0;
}
