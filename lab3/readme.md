# TCP version

client.cpp 和 server.cpp 

## Usage
g++ server.cpp -o server -lpthread

g++ client.cpp -o client -lpthread

sudo mn --custom lab3-topo.py --topo lab3topo

xterm h1

xterm h2

xterm h3

xterm h4

在h1里运行server，其他三个运行client

# UDP version

udpclient.cpp

## Usage 

g++ udpclient.cpp -o udpclient -lpthread

#### 旧的commit说明

"second ver, use process" 该版本使用fork导致sockfd查找表进程间不共享

"Finish TCP chat room" 该版本客户端和服务器一来一回创建了两条TCP连接，太傻了