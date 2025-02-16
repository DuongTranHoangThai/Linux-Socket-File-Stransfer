#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void stream_video(int client_socket, const char *filename) {
    FILE *file = fopen("video.mp4", "rb");  // Open video file in binary mode
    if (!file) {
        perror("Error opening video file");
        return;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, 0);  // Send video chunk
        usleep(30000);  // Simulate real-time streaming (30ms delay)
    }

    printf("Video stream finished.\n");
    fclose(file);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    
    printf("Waiting for client...\n");
    client_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    printf("Client connected. Streaming video...\n");

    stream_video(client_socket, "video.mp4");  // Send video
    close(client_socket);
    close(server_fd);
    
    return 0;
}

