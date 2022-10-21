#include<iostream>
#include<stdio.h>
#include<windows.h>
#include <ctime>
#include<string>
#include<map>
using namespace std;
#pragma  comment(lib,"ws2_32.lib")
SOCKET sockConn[1024];

SOCKADDR_IN addrClient = { 0 };
int len = sizeof(addrClient);
map<string, int>dict;
map<int, string>rdict;
static  int  num =0;
string gettime() {
	time_t now = time(0);
	char dt[26];
	ctime_s(dt, sizeof(dt), &now);
	string s = dt;
	return dt;
}
void trans(int id) {
	char buff[1024];
	string temp;
	string type0 = "!!!";
	string type1 = "@@@";//�����ʶ��

	while (1) {
		int r = recv(sockConn[id], buff, 1023, NULL);
		if (r > 0) {
			buff[r] = 0;// ���\0;
			string buff2 = buff;
			//cout << "log"<<buff2<<" " << buff2.find_first_not_of((type1)) << endl;
			if (buff2.find_first_not_of((type0))==3) {   //��������������ֺ�id��//���ڸտ�ʼ��id��������
				temp = buff2.substr(3);
				dict[temp] = id;//�����ֵ�
				rdict[id] = temp;// �������ֵ�//���ڲ���
				string time = gettime();
				string temptran = time+temp+ "����Ⱥ��";
				cout << "log: "<<time<<" " << temp << "����Ⱥ��" << endl;
				for (int i = 0; i < num; i++) {
					send(sockConn[i], temptran.c_str(), strlen(temptran.c_str()), NULL);

				}
				cout <<" Ⱥ����Ϣ�� " <<temptran << endl;
				//cout << "log" << temp << " " << id<<" "<< dict[temp] << endl;
			}
			else if (buff2.find_first_not_of((type1)) == 3) {////��@@@//ʵ�ֵ�������
				int flag = buff2.find("++");
				//cout << flag << endl;
				//cout << "debug:" << flag << endl;
				string tempname= buff2.substr(3,flag-3);
				//cout << "debug:" << tempname << endl;
				string content = buff2.substr(flag+2);
				//cout << "debug:" << content << endl;
				string time = gettime();
				string content2 = time+ rdict[id] + "���͸�" + content;
				send(sockConn[dict[tempname]], content.c_str(), strlen(content2.c_str()), NULL);
				cout <<"log: " << time<< rdict[id] + "������" + tempname + content << endl;;
			}
			else {
				//��ͨ�ź���Ⱥ�Ĺ㲥�㲥��
				for (int i = 0; i < num; i++) {
					send(sockConn[i], buff2.c_str(), strlen(buff2.c_str()), NULL);

				}
				cout<<"log: " << " Ⱥ����Ϣ�� " << buff << endl;
			}




		}
	}
};


void main() {
	//1����Э��汾
	cout << "welcome to chatroom";
	cout << "�������ӹ���" << endl;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//2����socket�汾
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//***
	if (sockSrv == -1) {
		cout << "����socketʧ��" << endl;
		WSACleanup();
		return;
	}
	cout << "����socket�ɹ�" << endl;


	// 3�� ip�Ͷ˿ں�
	SOCKADDR_IN addrSrv = { 0 }; //***�ṹ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//��һ��ipv4
	addrSrv.sin_port = htons(2048);//�˿ں�*** ��С��

	int bind1 = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(addrSrv));//ip��ַ�Ͷ˿ں� ����ַ���ȡ�
	if (bind1 == -1) {
		cout << "��ʧ��" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "�󶨳ɹ�" << endl;

	//4����
	int listen1 = listen(sockSrv, 10); //510���Ӷ��е���󳤶�
	if (listen1 == -1) {
		cout << "����ʧ��" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "�����ɹ�" << endl;

	//5�ͻ�������
	
	 
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
		 //���߳�ͨ��
		 CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)trans, (char*)num, NULL, NULL);
		 num++;
		 cout << "���ڵ�¼��������Ŀ�ǣ�" << num << endl;
		 delete[]dt;
	 }
	
	
	
	

	WSACleanup();


}