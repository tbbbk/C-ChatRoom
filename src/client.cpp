#include <iostream>
#include <thread>
#include <winsock2.h>
#include <string>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
bool EXIT = 1;

void recvfunc()
{
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�����׽���

	sockaddr_in serveraddr;//�׽��ֵ�ַ����

	serveraddr.sin_family = AF_INET;//ipv4
	serveraddr.sin_port = htons(6666);//�˿�
	serveraddr.sin_addr.s_addr = inet_addr("/*����ipv4��ַ*/");
	bind(server, (sockaddr*)&serveraddr, sizeof serveraddr);//�׽��ֳɹ���

	listen(server, 5);//����

	sockaddr_in clientaddr;//�ͻ������׽��ֵ�ַ
	int len = sizeof(clientaddr);
	SOCKET client = accept(server, (sockaddr*)&clientaddr, &len);//���տͻ���
	if (!client)
		cout << 1;
	char recvdata[1024] = { 0 };
	recv(client, recvdata, 1023, 0);//��������
	cout << "server:" << recvdata << endl;
}

int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;

    /* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
    wVersionRequested = MAKEWORD(2, 2);//�汾

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        /* Tell the user that we could not find a usable */
        /* Winsock DLL.                                  */
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    cout << "***һ���򵥵�����ϵͳ***" << endl;
    cout << "***һ������һ�У����ڱ������⣬������������***" << endl;
    cout << "***��������س��˳�***" << endl;
    cout << "::���ڽ��ܶԷ�����Ϣ������" << endl;


    //����
    thread recv = thread(recvfunc);


    //����
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
        SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�����׽���
        sockaddr_in serveraddr;
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr("/*������ip��ַ*/");
        serveraddr.sin_port = htons(6666);
        connect(client, (sockaddr*)&serveraddr, sizeof serveraddr);//����
        //�������˵�����
        send(client, (const char*)senddata, sizeof senddata, 0);//��������
        closesocket(client);
    }
    //�̻߳���
    recv.detach();
    recv.join();
    //����
    WSACleanup();
    return 0;
}