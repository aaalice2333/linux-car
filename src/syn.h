#ifndef _SYN_H
#define _SYN_H

#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<errno.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<string.h>
#include<stdlib.h>

using namespace std;

//全局变量
extern int total_number_of_cars;//汽车总数
extern int maximum_number_of_cars_in_tunnel;//隧道最大汽车容量
extern int tunnel_travel_time;//穿过隧道的时间
extern int total_number_of_mailboxes;//邮箱数量
extern int memory_segment_size;//每个邮箱的最大字数

//声明信号量操作,获取和释放；
//信号量函数实现
void wait(int sid, int sn);

void signal(int sid, int sn);


#endif
