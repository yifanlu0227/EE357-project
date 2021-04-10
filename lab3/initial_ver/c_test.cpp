#include<iostream>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#define SERVER_IP "10.0.0.1"
#define PORT "3490"
#define PORT2 "3491"
#define BACKLOG 20
#define LEN 100

using namespace std;
int main(){
    int sockfd;
    struct addrinfo hints,*res;

    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(SERVER_IP,PORT,&hints,&res);

    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    connect(sockfd,res->ai_addr,res->ai_addrlen);
    char msg[] = "Hello everyone!";
    
    int len,bytes_sent;
    len = strlen(msg);
    bytes_sent = send(sockfd,msg,len,0);
    char text[LEN];
    while(1){
        cout<<">>>";
        cin>>text;
        len = strlen(text);
        // sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
        // connect(sockfd,res->ai_addr,res->ai_addrlen);
        bytes_sent = send(sockfd,text,len,0);
        cout<<"byte_sent:"<<bytes_sent<<endl;
        // close(sockfd); 如果不close的话会不断增长
    }

    return 0;
}

