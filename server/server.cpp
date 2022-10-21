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
	string type1 = "@@@";//特殊标识符

	while (1) {
		int r = recv(sockConn[id], buff, 1023, NULL);
		if (r > 0) {
			buff[r] = 0;// 添加\0;
			string buff2 = buff;
			//cout << "log"<<buff2<<" " << buff2.find_first_not_of((type1)) << endl;
			if (buff2.find_first_not_of((type0))==3) {   //将！！！后的名字和id绑定//用于刚开始的id和姓名绑定
				temp = buff2.substr(3);
				dict[temp] = id;//建立字典
				rdict[id] = temp;// 建立逆字典//便于查找
				string time = gettime();
				string temptran = time+temp+ "进入群聊";
				cout << "log: "<<time<<" " << temp << "进入群聊" << endl;
				for (int i = 0; i < num; i++) {
					send(sockConn[i], temptran.c_str(), strlen(temptran.c_str()), NULL);

				}
				cout <<" 群发消息： " <<temptran << endl;
				//cout << "log" << temp << " " << id<<" "<< dict[temp] << endl;
			}
			else if (buff2.find_first_not_of((type1)) == 3) {////收@@@//实现单发功能
				int flag = buff2.find("++");
				//cout << flag << endl;
				//cout << "debug:" << flag << endl;
				string tempname= buff2.substr(3,flag-3);
				//cout << "debug:" << tempname << endl;
				string content = buff2.substr(flag+2);
				//cout << "debug:" << content << endl;
				string time = gettime();
				string content2 = time+ rdict[id] + "单送给" + content;
				send(sockConn[dict[tempname]], content.c_str(), strlen(content2.c_str()), NULL);
				cout <<"log: " << time<< rdict[id] + "单发给" + tempname + content << endl;;
			}
			else {
				//普通信号向群聊广播广播：
				for (int i = 0; i < num; i++) {
					send(sockConn[i], buff2.c_str(), strlen(buff2.c_str()), NULL);

				}
				cout<<"log: " << " 群发消息： " << buff << endl;
			}




		}
	}
};


void main() {
	//1请求协议版本
	cout << "welcome to chatroom";
	cout << "建立连接过程" << endl;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//2创建socket版本
	SOCKET sockSrv = socket(AF_INET, SOCK_STREAM, 0);//***
	if (sockSrv == -1) {
		cout << "创建socket失败" << endl;
		WSACleanup();
		return;
	}
	cout << "创建socket成功" << endl;


	// 3绑定 ip和端口号
	SOCKADDR_IN addrSrv = { 0 }; //***结构
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//第一个ipv4
	addrSrv.sin_port = htons(2048);//端口号*** 大小端

	int bind1 = bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(addrSrv));//ip地址和端口号 ，地址长度。
	if (bind1 == -1) {
		cout << "绑定失败" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "绑定成功" << endl;

	//4监听
	int listen1 = listen(sockSrv, 10); //510连接队列的最大长度
	if (listen1 == -1) {
		cout << "监听失败" << endl;
		cout << WSAGetLastError() << endl;
		closesocket(sockSrv);
		WSACleanup();
		return;
	}
	cout << "监听成功" << endl;

	//5客户端连接
	
	 
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
		 //多线程通信
		 CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)trans, (char*)num, NULL, NULL);
		 num++;
		 cout << "现在登录的主机数目是：" << num << endl;
		 delete[]dt;
	 }
	
	
	
	

	WSACleanup();


}