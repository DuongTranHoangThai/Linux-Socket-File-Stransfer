#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void receive_video(int socket_fd) {
    FILE *file = fopen("received_video.mp4", "wb");  // Save video stream
    if (!file) {
        perror("Error opening file for writing");
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    
    while ((bytes_received = recv(socket_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes_received, file);  // Write video data
    }

    printf("Video received successfully.\n");
    fclose(file);
}

int main() {
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("Connected to server, receiving video...\n");

    receive_video(sock);
    close(sock);

    system("vlc received_video.mp4");  // Automatically play the received video
    
    return 0;
}

