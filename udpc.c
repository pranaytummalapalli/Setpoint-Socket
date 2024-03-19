#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

int main(void){
    struct pollfd in_poll = {STDIN_FILENO, POLLIN|POLLPRI};
    struct timeval read_timeout;

    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    int socket_desc;
    struct sockaddr_in server_addr;
    // char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);

    char client_message[2000];
    
    // Create socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    // Get input from the user:
    printf("Enter message: ");
    while(1){
        if(poll(&in_poll, 1, 10)){
            scanf("%s", &client_message);
        }
        else{
        }

        // Send the message to server:
        if(sendto(socket_desc, client_message, sizeof(client_message), 0,
             (struct sockaddr*)&server_addr, server_struct_length) < 0){
            printf("Unable to send message\n");
            return -1;
        }
    }
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}
