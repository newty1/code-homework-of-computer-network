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
			buff[r] = 0;// 添加\0;
			//广播：
			for(int i=0;i<num;i++){
				send(sockConn[i], buff, strlen(buff), NULL);
				
			}
			cout << buff << endl;
			
			
		}

	}
}

void main() {
	//1请求协议版本
	cout << "welcome to chatroom";
	cout << "建立连接过程" << endl;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//创建socket版本
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//***
	if (sockSrv == -1) {
		cout << "创建socket失败" << endl;
		WSACleanup();
		return;
	}
	cout << "创建socket成功" << endl;


	// 绑定 ip和端口号
	SOCKADDR_IN addrSrv = { 0 }; //***结构
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//第一个ipv4
	addrSrv.sin_port = htons(10086);//端口号*** 大小端

	int bind1 = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(addrSrv));//ip地址和端口号 ，地址长度。
	if (bind1 == -1) {
		cout << "绑定失败" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "绑定成功" << endl;

	//监听
	int listen1 = listen(sockSrv, 10); //510连接队列的最大长度
	if (listen1 == -1) {
		cout << "监听失败" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "监听成功" << endl;

	//客户端连接
	
	 
	 while (1) {
		 sockConn[num] = accept(sockSrv, (SOCKADDR*)&addrClient, &len);//返回远程端地址，返回地址长度
		 if (sockConn[num] == -1) {
			 cout << "连接失败" << endl;
			 closesocket(sockSrv);
			 WSACleanup();
			 return;
		 }
		 char *dt= new char [26];
		 cout  << "客户端连接成功,ip地址是：" << inet_ntoa(addrClient.sin_addr) << endl;//输出地址** 转化成。。形式
		 CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)trans, (char*)num, NULL, NULL);
		 num++;
		 cout << "现在登录的主机数目是：" << num << endl;
		 delete[]dt;
	 }
	
	//通信
	
	

	WSACleanup();


}