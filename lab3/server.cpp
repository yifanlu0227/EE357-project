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
    // create socket descriptor
    // this is the old way
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

    while(1){
        struct sockaddr_storage their_addr;
        socklen_t addr_size = sizeof(their_addr);
        int new_fd = accept(sockfd,(struct sockaddr*) &their_addr,&addr_size);
        if(new_fd == -1){
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr*)&their_addr),s,sizeof s);
        cout<<"Server: got connection from "<<s<<endl;

        if(!fork()){ // pid=0, child process 
            close(sockfd);
            char buf[LEN];
            char prefix[LEN];
            strcpy(prefix,s);
            memset(buf,'\0',LEN);

            int recv_len = recv(new_fd,buf,LEN,0);

            // indicating server have received the message
            if(recv_len==-1){
                cout<<"receive"<<endl;
                break;
            }
            else if(recv_len==0){
                cout<<"Closed by remote side."<<endl;
                break;
            }
            else{
                cout<<"Received:"<<buf<<endl;
            }
            strcat(prefix," says: ");
            strcat(prefix,buf);

            // broadcast to other clients
            for(int i=0;i<3;i++){
                if(strcmp(s,clients[i])==0)
                    continue;
                int sendfd;
                sendfd = socket(AF_INET,SOCK_STREAM,0);
                struct sockaddr_in sendaddr;
                sendaddr.sin_family = AF_INET;
                sendaddr.sin_port = htons(PORT2);
                sendaddr.sin_addr.s_addr = inet_addr(clients[i]);
                memset(sendaddr.sin_zero,'\0',sizeof sendaddr.sin_zero);
                // reach here
                if(connect(sendfd,(struct sockaddr*)&sendaddr,sizeof(sendaddr))==-1){
                    perror("connect");
                }

                int bytes_sent = send(sendfd,prefix,strlen(prefix),0);
                cout<<"sent byte "<<bytes_sent<<endl;
                close(sendfd);
            }
            close(new_fd);
            exit(0);
        }
        else{
            int status;
            wait(&status);
            close(new_fd);
        }

    }

	return 0;
}
