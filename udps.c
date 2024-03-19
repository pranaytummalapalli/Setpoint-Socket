#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void){
    int socket_desc;
    struct sockaddr_in server_addr, client_addr;
    char recieved_buf[2000];
    long setpoint[2];
    struct timeval read_timeout;

    read_timeout.tv_sec = 0;
    read_timeout.tv_usec = 10;
    
    
    int client_struct_length = sizeof(client_addr);
    memset(recieved_buf, '\0', sizeof(recieved_buf));
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
    while(1){
     if (recvfrom(socket_desc, recieved_buf, sizeof(recieved_buf), 0,
         (struct sockaddr*)&client_addr, &client_struct_length) < 0){
        printf("Couldn't receive\n");
    }
    printf("Rec string: %s \n", recieved_buf);
    char * token = strtok(recieved_buf, ",");
    printf("Rec2 string: %s \n", token);

    // printf("recieved message: %ld \n", setpoint[0]);
    // printf("recieved message: %ld \n", setpoint[1]);
    }
    close(socket_desc);
    
    return 0;
}