#include<iostream>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<pthread.h>
#define SERVER_IP "10.0.0.1"
#define PORT "3490"
#define BACKLOG 20
#define LEN 100
using namespace std;
int sockfd;

static void * pthread(void *arg){
    while(1){
        char buf[LEN];
        int recv_len = recv(sockfd,buf,LEN,0);
        if(recv_len==0 || recv_len==-1)
            return NULL;
        cout<<"<<< "<<buf<<endl;
        memset(buf,'\0',LEN);
    }
    return NULL;
}



int main(){
    // pid_t pid;
    // pid = fork();
    // if(pid==0){ //child process, used for listening
    //     int sockfd;
    //     struct addrinfo hints,*res;
    //     memset(&hints,0,sizeof hints);
    //     hints.ai_family = AF_INET;
    //     hints.ai_socktype = SOCK_STREAM;
    //     hints.ai_flags = AI_PASSIVE;
    //     getaddrinfo(NULL,PORT,&hints,&res);
    //     sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    //     bind(sockfd,res->ai_addr,res->ai_addrlen);
    //     if(listen(sockfd,BACKLOG) == -1){
    //         perror("listen");
    //         exit(1);
    //     }
    //     struct sockaddr_storage their_addr;
    //     socklen_t addr_size = sizeof(their_addr);
    //     int new_fd = accept(sockfd,(struct sockaddr*) &their_addr,&addr_size);
    //     if(new_fd == -1){
    //         perror("accept");
    //         exit(0);
    //     }
    //     while(1){
    //         char buf[LEN];
    //         int recv_len = recv(new_fd,buf,LEN,0);
    //         cout<<"<<< "<<buf<<endl;
    //         memset(buf,'\0',LEN);
    //     }
    // }

    
    struct addrinfo hints,*res;

    memset(&hints,0,sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(SERVER_IP,PORT,&hints,&res);

    sockfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    connect(sockfd,res->ai_addr,res->ai_addrlen);
    char msg[] = "Hello everyone!";
    // char hostname[LEN] = {0};
    // gethostname(hostname,LEN);
    // struct hostent *hent = gethostbyname(hostname);
    // char *ip = inet_ntoa(*(struct in_addr*)(hent->h_addr_list[0]));
    // cout<<ip<<endl;
    
    int len,bytes_sent;
    len = strlen(msg);
    bytes_sent = send(sockfd,msg,len,0);
    
    pthread_t tid;
    if ((pthread_create(&tid, NULL, pthread, NULL)) == -1){
        cout<<("create error!\n");
        return -1;
    }
    cout<<"Thread created."<<endl; 

    char text[LEN];
    while(1){
        memset(text,'\0',LEN);
        cout<<">>>";
        cin.getline(text,LEN);
        len = strlen(text);
        bytes_sent = send(sockfd,text,len,0);
        // cout<<"byte_sents "<<bytes_sent<<endl;
    }
  

    return 0;
}

