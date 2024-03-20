#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>
#include <sys/time.h>

int main(void){
    struct pollfd in_poll = {STDIN_FILENO, POLLIN|POLLPRI};
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    
    long server_data[1];
    int ack[1];
    struct timeval read_timeout;

    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 100;
    
    int client_struct_length = sizeof(client_addr);
    int server_struct_length = sizeof(server_addr);
    memset(server_data, '\0', sizeof(server_data));

    // Create UDP socket:
    socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    setsockopt(socket_desc, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
    
    if(socket_desc < 0){
        printf("Error while creating socket\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(2000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        printf("Couldn't bind to the port\n");
        return -1;
    }

    printf("Enter message: ");
    while(1){
        if(poll(&in_poll, 1, 10)){
            scanf("%ld", &server_data[0]);
        }
        else{
        }
        if(recvfrom(socket_desc, ack, sizeof(ack), MSG_WAITALL, (struct sockaddr *)&client_addr, &client_struct_length) < 0){
            perror("rec failed");
        }
        if(sendto(socket_desc, server_data, sizeof(server_data), 0,
             (struct sockaddr*)&client_addr, client_struct_length) < 0){
            perror("Unable to send message");
            // return -1;
        }
    }
    close(socket_desc);
    
    return 0;
}