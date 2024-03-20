#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <sys/time.h>

int main(void){
    // struct pollfd in_poll = {STDIN_FILENO, POLLIN|POLLPRI};
    struct timeval read_timeout;

    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    // char server_message[2000], data[2000];
    int server_struct_length = sizeof(server_addr);
    int client_struct_length = sizeof(client_addr);

    long data[1];
    int ack[1] = {1771};
    
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


    // if(connect(socket_desc, (struct spckaddr *)&server_addr, server_struct_length) < 0){
    //     perror("Failed connect");
    // }
    
    while(1){
        if(sendto(socket_desc, ack, sizeof(ack), 0, (struct sockaddr *)&server_addr, &server_struct_length) < 0){
            perror("Ack failed");
        }
        // Send the message to server:
        if (recvfrom(socket_desc, data, sizeof(data), 0,
         (struct sockaddr*)&server_addr, &server_struct_length) < 0){
        perror("Couldn't receive");
        }
        printf("recv data: %ld\n", data[0]);
    }
    
    // Close the socket:
    close(socket_desc);
    
    return 0;
}
