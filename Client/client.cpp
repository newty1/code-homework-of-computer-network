#include<iostream>
#include<windows.h>
#include <ctime>
#include<string>
using namespace std;
#pragma  comment(lib,"ws2_32.lib")
SOCKET sockCli;
int type(string s1) {
	if (s1 == "rename")
	{
		return 1;
	}
	else if (s1 == "quit")
	{
		return 2;
	}
	else if (s1 == "author")
	{
		return 3;
	}
	else if (s1=="help") {
		return 4;
	}
	else {
		return 0;
	}

};
void refroS() {
	char recvbuf[1024];
	int result;
	while (1) {
		result = recv(sockCli, recvbuf, 1023, NULL);//�յ�����Ϣ
		if (result > 0) {
			recvbuf[result] = 0;/**/
			cout << "�յ���Ϣ�� " << endl;
			cout << recvbuf << endl;
		}
	}
}
string gettime() {
	time_t now = time(0);
	char dt[26];
	ctime_s(dt, sizeof(dt), &now);
	string s = dt;
	return dt;
}
int port; string ip, name;
void main() {
	cout << "welocme to chatclient" << endl;

	
	cout << "�������û���";
	cin >> name;

	cout << "��ӭ�㣺"  << name<< endl;
	cout << "������ip �Ƽ�ʹ�� 127.0.0.1 " << endl;
	cin >> ip;
	cout << "������˿ں� �Ƽ�ʹ��2048" << endl;
	cin >> port;
	//1����Э��汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);//��ʼ��socketdll
	//������ϣ��ʹ�õ���߰汾 2.2�汾������socket����Ϣ
	
	
	//����socket�汾
	 sockCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);// ��ַ���ͣ��������ͣ�Э������***
	if (sockCli == -1) {
		cout << "����socketʧ��" << endl;
		WSACleanup();
		return   ;
	}
	cout << "����socket�ɹ�" << endl;
													
													
	//�� ip�Ͷ˿ں�
	SOCKADDR_IN addSrv ={ 0 }; //***�ṹ
	addSrv.sin_family = AF_INET;
	addSrv.sin_addr.S_un.S_addr = inet_addr(ip.data());//��һ��ipv4
	addSrv.sin_port = htons(port);//�˿ں�*** ��С��


	//���ӷ�����
	int con = connect(sockCli, (sockaddr*)&addSrv, sizeof(addSrv));
		//��һ��tidings��socket�����������ӵ�����
		if (con == -1) {
			cout << "���ӷ�����ʧ��" << endl;
			WSACleanup();
			return;
		}
	cout << "���ӷ������ɹ�" << endl;
		//ͨ��
	// ������Ϣ
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)refroS, NULL, NULL, NULL);
	//�����̣߳��ܷ񱻼̳У���ջ��ʼ��С��
	//�̺߳�����ʼ��ַ�����ݸ��̵߳Ĳ�����
	//�����̵߳ı�־��ָ���̵߳�ָ��
	string buff, time,name2;
	//����
	while (1) {
		cout << "$$:" << endl;
		cin >> buff;
		//cout << type(buff);
		switch (type(buff))
		{
		case 1: cout << "�������µ�����" << endl;
			name2 = name;
			cin >> name;
			buff = name2 + " rename to " + name;
			break;
		case 2: cout << " �˳�" << endl;
			buff = name+"�˳�";
			exit(0);
		case 3:
			cout << "this is by Yan2013475" << endl;
		}
		string time = gettime();
		if (type(buff) == 3 or type(buff) == 4)
		{
			;
		}
		
		else if(type(buff)==1) {
			buff = time + name + " : " + buff;
			send(sockCli, buff.c_str(), strlen(buff.c_str()), 0);
		}
		else {
		buff = time + name + " : " + buff;
		send(sockCli, buff.c_str(), strlen(buff.c_str()), 0);
		
	}
	}
		
	closesocket(sockCli);
	WSACleanup();//����ʹ��socket���ͷ�socket dll


}

