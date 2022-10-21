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
		result = recv(sockCli, recvbuf, 1023, NULL);//收到的消息
		if (result > 0) {
			recvbuf[result] = 0;/**/
			cout << "收到消息： " << endl;
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

	
	cout << "请输入用户名";
	cin >> name;

	cout << "欢迎你："  << name<< endl;
	cout << "请输入ip 推荐使用 127.0.0.1 " << endl;
	cin >> ip;
	cout << "请输入端口号 推荐使用2048" << endl;
	cin >> port;
	//1请求协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);//初始画socketdll
	//调用者希望使用的最高版本 2.2版本，可用socket的信息
	
	
	//创建socket版本
	 sockCli = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);// 地址类型，服务类型，协议类型***
	if (sockCli == -1) {
		cout << "创建socket失败" << endl;
		WSACleanup();
		return   ;
	}
	cout << "创建socket成功" << endl;
													
													
	//绑定 ip和端口号
	SOCKADDR_IN addSrv ={ 0 }; //***结构
	addSrv.sin_family = AF_INET;
	addSrv.sin_addr.S_un.S_addr = inet_addr(ip.data());//第一个ipv4
	addSrv.sin_port = htons(port);//端口号*** 大小端


	//连接服务器
	int con = connect(sockCli, (sockaddr*)&addSrv, sizeof(addSrv));
		//向一个tidings的socket发出建立连接的请求
		if (con == -1) {
			cout << "连接服务器失败" << endl;
			WSACleanup();
			return;
		}
	cout << "连接服务器成功" << endl;
		//通信
	// 接受消息
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)refroS, NULL, NULL, NULL);
	//创建线程，能否被继承，堆栈初始大小，
	//线程函数开始地址，传递给线程的参数，
	//控制线程的标志，指向线程的指针
	string buff, time,name2;
	//发送
	while (1) {
		cout << "$$:" << endl;
		cin >> buff;
		//cout << type(buff);
		switch (type(buff))
		{
		case 1: cout << "请输入新的名字" << endl;
			name2 = name;
			cin >> name;
			buff = name2 + " rename to " + name;
			break;
		case 2: cout << " 退出" << endl;
			buff = name+"退出";
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
	WSACleanup();//结束使用socket，释放socket dll


}

