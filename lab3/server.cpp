#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<errno.h>
#include<stdio.h>
#include<string.h>
#include<string>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<map>

#define PORT "3490"
#define PORT2 "3491"
#define BACKLOG 20
#define LEN 100
#define MAX_CLIENT 20
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
    int sockfd,client_num=0;
    map<string,int> recvfd_table;
    map<string,int> sendfd_table;


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
        int new_fd = accept(sockfd,(struct sockaddr*) &their_addr,&addr_size); // 
        if(new_fd == -1){
            perror("accept");
            continue;
        }
        char s[LEN];
        inet_ntop(their_addr.ss_family,get_in_addr((struct sockaddr*)&their_addr),s,sizeof s);
        string ip_addr = s;
        map<string,int>::iterator iter = recvfd_table.find(s);

        if(iter==recvfd_table.end()){
            recvfd_table.insert(pair<string,int>(ip_addr,new_fd));
            cout<<"Server: got new connection from "<<ip_addr<<endl;
            // echo for acknowledge
            struct addrinfo hints,*res;
            memset(&hints,0,sizeof hints);
            hints.ai_family = AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            getaddrinfo(ip_addr.c_str(),PORT2,&hints,&res);
            int sendfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
            connect(sendfd,res->ai_addr,res->ai_addrlen);
            sendfd_table.insert(pair<string,int>(ip_addr,sendfd));
            char text[] = "Acknowledge from server. \0";
            int byte_send = send(sendfd,text,strlen(text),0);
            // cout<<"byte send in ack:"<<byte_send<<endl;
        }

        pid_t pid;
        pid = fork();

        if(pid==0){ // pid=0, child process, keep connection with new_fd above. Once received message, handout to other client
            char buf[LEN];
            string client_ip = ip_addr;
            while(1){
                memset(buf,'\0',LEN);
                int recv_len = recv(new_fd,buf,LEN,0);

                // indicating server have received the message
                if(recv_len==-1){cout<<"receive failed."<<endl; break;}
                else if(recv_len==0){cout<<"Closed by remote side."<<endl; 
                    close(sendfd_table[client_ip]);
                    close(recvfd_table[client_ip]);
                    sendfd_table.erase(client_ip);
                    recvfd_table.erase(client_ip);
                    break;
                }
                else {cout<<"Received:"<<buf<<endl;}

                string text = buf;
                string prefix = client_ip;
                string sent_message = prefix + " says: "+ text;

                
                int sendfd;
                // broadcast to other clients
                for(map<string,int>::iterator iter=sendfd_table.begin();iter!=sendfd_table.end();iter++){
                    if(client_ip == iter->first)
                        continue;
                    sendfd = iter->second;
                    cout<<sent_message<<endl;
                    int bytes_sent = send(sendfd,sent_message.c_str(),sent_message.length(),0);
                    cout<<"sent byte "<<bytes_sent<<" to "<<iter->first<<" with sockfd "<<iter->second<<endl;
                }
            }
        }

    }

	return 0;
}
