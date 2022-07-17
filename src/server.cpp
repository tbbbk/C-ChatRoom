#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <thread>
using namespace std;

void Send()
{
        while(true)
        {
                cout<<":";
                int client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字

                sockaddr_in serveraddr;
                serveraddr.sin_family = AF_INET;
                serveraddr.sin_addr.s_addr = inet_addr("/*主机地址*/");
                serveraddr.sin_port = htons(6666);

                connect(client, (sockaddr*)&serveraddr, sizeof serveraddr);//连接

                char senddata[1024] = {0};
                cin.getline(senddata,1024);
                send(client, senddata, sizeof senddata, 0);//发送数据
        }
}

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字

    sockaddr_in serveraddr;//套接字地址类型

    serveraddr.sin_family = AF_INET;//ipv4
    serveraddr.sin_addr.s_addr = inet_addr("/*服务器内网地址*/");//本机IP地址
    serveraddr.sin_port = htons(6666);

    bind(sockfd, (sockaddr*)&serveraddr, sizeof serveraddr);//套接字成功绑定

    listen(sockfd,5);//监听

    int epfd=epoll_create1(0);
    epoll_event events[1024],ev;
    ev.events =EPOLLIN;
    ev.data.fd=sockfd;
    epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev);
    thread senD=thread(Send);

    while(1)
    {
        int nfds=epoll_wait(epfd,events,1024,-1);
        for(int i=0;i<nfds;i++)
        {
                if(events[i].data.fd==sockfd)
                {
                        sockaddr_in clientaddr;
                        socklen_t len=sizeof(clientaddr);
                        int client_sockfd = accept(sockfd, (sockaddr*)&clientaddr, &len);
                        printf("new client fd %d! IP: %s Port: %d\n", client_sockfd, inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
                        //连接完毕
                        ev.data.fd=client_sockfd;
                        ev.events=EPOLLIN;
                        epoll_ctl(epfd,EPOLL_CTL_ADD,client_sockfd,&ev);
                }
                else
                {
                        char recvdata[1024]={0};
                        recv(events[i].data.fd, recvdata, sizeof recvdata, 0);//接受数据
                        cout<<events[i].data.fd<<" : "<<recvdata<<endl;
                        close(events[i].data.fd);
                }
                //close(events[i].data.fd);
        }
    }
    senD.join();
    close(sockfd);
    return 0;
}