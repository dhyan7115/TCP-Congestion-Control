#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#define PORT 5000
void handle_congestion_control(int threshold,int timeout){
    int cwnd = 1;
    printf("Initial CWND Value : %d\n",cwnd);
    printf("Initial Threshold Value : %d\n",threshold);
    while(cwnd<threshold){
        cwnd = cwnd*2;
        printf("CWND : %d\n",cwnd);
        sleep(1);
        if(cwnd>=timeout){
            cwnd = 1;
            threshold = threshold/2;
            printf("CWND After Timeout : %d\n",cwnd);
            printf("Threshold Value after Timeout : %d\n",threshold);
            break;
        }
    }
    while(1){
        cwnd = cwnd+1;
        printf("CWND : %d\n",cwnd);
        sleep(1);
    }
}
int main(){
    int client_socket;
    int server_socket;
    struct sockaddr_in address;
    socklen_t addr_len = sizeof(address);
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);
    bind(server_socket,(struct sockaddr*)&address,addr_len);
    listen(server_socket,10);
    printf("Server is Listening to Port %d\n",PORT);
    client_socket = accept(server_socket,(struct sockaddr*)&address,&addr_len);
    printf("Connected to the Client\n");
    int threshold, timeout;
    int bytes = recv(client_socket,&threshold,sizeof(int),0);
    bytes = recv(client_socket,&timeout,sizeof(int),0);
    handle_congestion_control(threshold,timeout);
    close(client_socket);
    close(server_socket);
    return 0;
}