#include<iostream>
#include<stdio.h>
#include<windows.h>
#include <ctime>
#include<string>
using namespace std;
#pragma  comment(lib,"ws2_32.lib")
SOCKET sockConn[1024];
SOCKADDR_IN addrClient = { 0 };
int len = sizeof(addrClient);
static  int  num =0;
//char* gettime(char* dt) {
//	time_t now = time(0);
//	
//	ctime_s(dt, sizeof(dt), &now);
//	return dt;
//}
void trans(int id) {
	char buff[1024];
	while (1) {
		int r = recv(sockConn[id], buff, 1023, NULL);
		if (r > 0) {
			buff[r] = 0;// ���\0;
			//�㲥��
			for(int i=0;i<num;i++){
				send(sockConn[i], buff, strlen(buff), NULL);
				
			}
			cout << buff << endl;
			
			
		}

	}
}

void main() {
	//1����Э��汾
	cout << "welcome to chatroom";
	cout << "�������ӹ���" << endl;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//����socket�汾
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//***
	if (sockSrv == -1) {
		cout << "����socketʧ��" << endl;
		WSACleanup();
		return;
	}
	cout << "����socket�ɹ�" << endl;


	// �� ip�Ͷ˿ں�
	SOCKADDR_IN addrSrv = { 0 }; //***�ṹ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//��һ��ipv4
	addrSrv.sin_port = htons(10086);//�˿ں�*** ��С��

	int bind1 = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(addrSrv));//ip��ַ�Ͷ˿ں� ����ַ���ȡ�
	if (bind1 == -1) {
		cout << "��ʧ��" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "�󶨳ɹ�" << endl;

	//����
	int listen1 = listen(sockSrv, 10); //510���Ӷ��е���󳤶�
	if (listen1 == -1) {
		cout << "����ʧ��" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "�����ɹ�" << endl;

	//�ͻ�������
	
	 
	 while (1) {
		 sockConn[num] = accept(sockSrv, (SOCKADDR*)&addrClient, &len);//����Զ�̶˵�ַ�����ص�ַ����
		 if (sockConn[num] == -1) {
			 cout << "����ʧ��" << endl;
			 closesocket(sockSrv);
			 WSACleanup();
			 return;
		 }
		 char *dt= new char [26];
		 cout  << "�ͻ������ӳɹ�,ip��ַ�ǣ�" << inet_ntoa(addrClient.sin_addr) << endl;//�����ַ** ת���ɡ�����ʽ
		 CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)trans, (char*)num, NULL, NULL);
		 num++;
		 cout << "���ڵ�¼��������Ŀ�ǣ�" << num << endl;
		 delete[]dt;
	 }
	
	//ͨ��
	
	

	WSACleanup();


}