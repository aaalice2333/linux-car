#include"tunnel.h"
#include <time.h>  
#include <sys/time.h>

 //汽车类
class car {
	int no;//汽车的编号
	int time;//汽车穿越隧道使用的时间
	int state;//汽车的当前状态，1未进入隧道，2已经隧道中，3出隧道
	string str;//手机内存
	int sid;//信号量，规定隧道里最大车辆数目
	vector<int> m;//每个邮箱维护的读指针

public:

	car(int n,int sid1) 
	{
		no = n;
		state = 1;
		time = 0;
		str = " ";
		sid = sid1;
		m.resize(total_number_of_mailboxes);
		for (int i = 0; i < total_number_of_mailboxes; i++)
			m[i] = 0;
	}

	//判断是不是超过隧道运行的最大时间
	bool overtime(int t);
	//汽车进入隧道
	bool car_into_tunnel(int &usetime);
	//汽车出隧道
	bool car_out_tunnel();
	//汽车写消息,T隧道，no1邮箱序号，s字符串，n字符串长度，t时间
	bool car_write(tunnel *T, int num, char *s, int n, int t);
	//汽车读消息，T隧道，no1邮箱序号，s字符串，n字符串长度，t时间
	bool car_read(tunnel *T, int num, char* &s, int n, int t);
};