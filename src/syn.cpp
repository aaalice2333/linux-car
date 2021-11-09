#include"syn.h"

//信号量函数实现

//信号量Wait函数
void wait(int sid, int sn) {
	struct sembuf op;
	op.sem_num = sn;
	op.sem_op = -1;
	op.sem_flg = 0;
	semop(sid, &op, 1);
	return;
}

//信号量Signal函数
void signal(int sid, int sn) {
	struct sembuf op;
	op.sem_num = sn;
	op.sem_op = 1;
	op.sem_flg = 0;
	semop(sid, &op, 1);
	return;
}

