#include <iostream>
#include <thread>
#include <winsock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
bool EXIT = 1;

void recvfunc()
{
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字

	sockaddr_in serveraddr;//套接字地址类型

	serveraddr.sin_family = AF_INET;//ipv4
	serveraddr.sin_port = htons(6666);//端口
	serveraddr.sin_addr.s_addr = inet_addr("/*本机ipv4地址*/");
	bind(server, (sockaddr*)&serveraddr, sizeof serveraddr);//套接字成功绑定

	listen(server, 5);//监听

	sockaddr_in clientaddr;//客户机的套接字地址
	int len = sizeof(clientaddr);
	SOCKET client = accept(server, (sockaddr*)&clientaddr, &len);//接收客户机
	if (!client)
		cout << 1;
	char recvdata[1024] = { 0 };
	recv(client, recvdata, 1023, 0);//接受数据
	cout << "server:" << recvdata << endl;
}

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);//版本

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    cout << "***一个简单的聊天系统***" << endl;
    cout << "***一次输入一行，由于编码问题，请勿输入中文***" << endl;
    cout << "***单次输入回车退出***" << endl;
    cout << "::正在接受对方的消息。。。" << endl;


    //接收
    thread recv = thread(recvfunc);


    //发送
    while (1)
    {
        cout << "client:";
        string tbk={""};
        getline(cin, tbk);
        if (tbk == "")
            break;
        char senddata[1024] = { 0 };
        for (int i = 0; i < tbk.length(); i++)
            senddata[i] = tbk[i];
        SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建套接字
        sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr("/*服务器ip地址*/");
        serveraddr.sin_port = htons(6666);
        connect(client, (sockaddr*)&serveraddr, sizeof serveraddr);//连接
        //服务器端的数据
        send(client, (const char*)senddata, sizeof senddata, 0);//发送数据
        closesocket(client);
    }
    //线程回收
    recv.detach();
    recv.join();
    //结束
    WSACleanup();
    return 0;
}