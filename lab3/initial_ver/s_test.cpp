#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netdb.h>

#define PORT "3490"
#define PORT2 3491
#define CLIENT_IP1 "10.0.0.2";
#define CLIENT_IP2 "10.0.0.3";
#define CLIENT_IP3 "10.0.0.4";
#define BACKLOG 20
#define LEN 100
using namespace std;

void *get_in_addr(struct sockaddr *sa){
    if(sa->sa_family == AF_INET){
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


int main(int argc,char *argv[]){
    struct addrinfo hints,*res;
    int sockfd;
    char s[INET6_ADDRSTRLEN];
    char **clients;
    clients = new char*[3];
    for(int i=0;i<3;i++){
        clients[i] = new char[20];
        strcpy(clients[i], "10.0.0.2");
        clients[i][7] += i; 
    }
    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // refer to self.

    getaddrinfo(NULL,PORT,&hints,&res);
    // make a socket
    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);

    bind(sockfd,res->ai_addr,res->ai_addrlen);

    if(listen(sockfd,BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    cout<<"Waiting for connection"<<endl;
    // accept an incoming connection
    // Structure large enough to hold any socket address


    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof(their_addr);
    cout<<"waiting!!"<<endl;
    while(1){

        int new_fd = accept(sockfd,(struct sockaddr*) &their_addr,&addr_size);
        cout<<"accepted!!"<<endl;
        if(new_fd == -1){
            perror("accept");
        }

        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr*)&their_addr),s,sizeof s);
        cout<<"Server: got connection from "<<s<<endl;
        char buf[LEN];
        int recv_len = recv(new_fd,buf,LEN,0);

        // indicating server have received the message
        char msg[] = "Hello I am server test!";
    
        int len,bytes_sent;
        len = strlen(msg);
        bytes_sent = send(new_fd,msg,len,0);
        cout<<"server sent bytes:"<<bytes_sent<<endl;

        close(new_fd);
    }

	return 0;
}
