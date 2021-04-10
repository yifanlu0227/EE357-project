client.cpp 和 server.cpp 是TCP连接的初始版本，每次连接都要新的sock，将要写新的版本会保存新加入进来的客户端的sockfd

## Usage
g++ server.cpp -o server
g++ client.cpp -o client

sudo mn --custom lab3-topo.py --topo lab3topo
xterm h1
xterm h2
xterm h3
xterm h4

在h1里运行server，其他三个运行client