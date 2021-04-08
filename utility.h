
#define UTILITY_H_INCLUDED

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// 保存所有的生成的socket
list<int> clients_list;

#define SERVER_IP "127.0.0.1"

#define SERVER_PORT 8888

#define EPOLL_SIZE 5000

#define BUF_SIZE 0xFFFF

#define SERVER_WELCOME "Welcome you join  to the chat room! Your chat ID is: Client #%d"

#define SERVER_MESSAGE "ClientID %d say >> %s"

#define EXIT "EXIT"

#define CAUTION "There is only one int the char room!"


int setnonblocking(int sockfd)
{
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)| O_NONBLOCK);
    return 0;
}


void addfd( int epollfd, int fd, bool enable_et )
{
    struct epoll_event ev;
    ev.data.fd = hfd;
    ev.events = EPOLLIN;
    if( enable_et )
        ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblocking(fd);
    printf("fd added to epoll!\n\n");
}


int sendBroadcastmessage(int clientfd)
{
    
    char buf[BUF_SIZE], message[BUF_SIZE];
    bzero(buf, BUF_SIZE);
    bzero(message, BUF_SIZE);

 
    printf("read from client(clientID = %d)\n", clientfd);
    int len = recv(clientfd, buf, BUF_SIZE, 0);

    if(len == 0)  // 当长度为零时，关闭client
    {
        close(clientfd);
        clients_list.remove(clientfd); //移除client
        printf("ClientID = %d closed.\n now there are %d client in the char room\n", clientfd, (int)clients_list.size());

    }
    else  //向socket列表中的所有socket发送信息
    {

        list<int>::iterator it;
        for(it = clients_list.begin(); it != clients_list.end(); ++it) {
           if(*it != clientfd){
                if( send(*it, message, BUF_SIZE, 0) < 0 ) { perror("error"); exit(-1);}
           }
        }
    }
    return len;
}
#endif 
