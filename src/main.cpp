

#include"car.h"

int total_number_of_cars=0;//汽车总数
int maximum_number_of_cars_in_tunnel=0;//隧道最大汽车容量
int tunnel_travel_time=0;//穿过隧道的时间
int total_number_of_mailboxes=0;//邮箱数量
int memory_segment_size=0;//每个邮箱的最大字数

//读入每一辆汽车的命令
bool input_car(ifstream &file,stringstream &buf)
{
    string s;
    while (file >> s)
    {
        if (s=="end")
        {
            buf << s;
            return 1;
        }
        else if (s=="car")
        {
            buf << s << " ";
            file >> s;
            buf << s << " ";
        }
        else
        {
            buf << s << " ";
            file >> s;
            buf << s << " ";
            file >> s;
            buf << s << " ";
            file >> s;
            buf << s << " ";
        }
    }
    return 1;
}


//输入每个车的配置，并处理
void deal_car(tunnel *T, stringstream &file,int sid)
{
    string str;
    int flag=0;//标记进隧道的第一个操作
    file >> str;
    if (str=="car")
    {
        int time_out = 1;//是否超时，1为否
        int no;
        int time;
        file >> no;
        car* Car = new car(no, sid);

        Car->car_into_tunnel(time);
        //printf("return time %d\n",time);

        while (file >> str)
        {
            if (str=="end")//如果输入结束，直接结束
            {
                if (time_out)//如果没有超时，出隧道
                    Car->car_out_tunnel();
                return ;
            }
            else if (str=="w")//写入
            {
                string st;
                int t;
                int n;
                char* s;
                char tmp;
                int len;
                file >> tmp >> st >> t >> n;//字符串，时间，第几个邮箱
                if(flag==0)
                {
                    flag=1;
                    t=t-time;//减去进入隧道的时间
					//printf("newtime:%d\n",t);
					if(t<0)
					{
						printf("time error: pass time can't be negative(%d)\n",t);
						exit(0);
					}
                }
                strcpy(s,st.c_str());
                len = st.length();
                s[len-1]='\0';//去掉后面的引号
                st[len-1]='\0';
                len=len-1;
                if (Car->overtime(t)==1&&time_out==1)//如果来得及执行操作，并且没有超时
                {
                    Car->car_write(T, n, s, len,t);
                }
                else
                {
                    if (time_out==1)//如果来不及执行的话
                    {
                        Car->car_out_tunnel();
                        time_out = 0;
                    }
                    Car->car_write(T, n, s, len,t);
                }
            }
            else if (str=="r")//读入
            {
                string st;
                int t;
                int n;
                char* s=new char[memory_segment_size];
                int len;
                file >> len >> t >> n;
                if(flag==0)
                {
                    flag=1;
                    t=t-time;//减去进入隧道的时间
					if(t<0)
					{
						printf("time error: pass time can't be negative(%d)\n",t);
						exit(0);
					}
                }
                if (Car->overtime(t)==1&& time_out==1)
                {
					//printf("sssss %s\n",str.c_str());
                    Car->car_read(T, n, s, len, t);
                }
                else
                {
                    if (time_out==1)
                    {
                        Car->car_out_tunnel();
                        time_out = 0;
                    }
                    Car->car_read(T, n, s, len, t);
                }
            }
        }
    }
    return ;
}

int main(int argc, char ** argv)
{
    //创建文件流
    ifstream file(argv[1]);
    if (!file.good())
    {
		printf("File open error!\n");
        return 0;
    }

    //读入输入数据
    file >> total_number_of_cars >> maximum_number_of_cars_in_tunnel >> tunnel_travel_time >> total_number_of_mailboxes >> memory_segment_size;
    cout << "total_number_of_cars：" << total_number_of_cars << endl;
    cout << "maximum_number_of_cars_in_tunnel：" << maximum_number_of_cars_in_tunnel << endl;
    cout << "tunnel_travel_time：" << tunnel_travel_time << endl;
    cout << "total_number_of_mailboxes：" << total_number_of_mailboxes << endl;
    cout << "memory_segment_size：" << memory_segment_size << endl<<endl;

    //字符串流用于缓存一个车的配置
    stringstream buf;

    //system_v信号量函数,创建一个信号量集，用于控制邮箱的写操作的独立性
    int semid = semget(IPC_PRIVATE, total_number_of_mailboxes, IPC_CREAT | 0666);
    //IPC_PRIVATE，新建对象
    //信号量的个数等于邮箱数量
    //IPC_CREAT，如果信号量不存在就创建一个新的信号量，如果存在则用错误返回该信号量的标识IPC_EXCL
    if (semid == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Mailbox semaphore set creation error!\n");
        exit(0);
    }
    //对第i个信号量进行SETVAL操作，值为1
    for (int i=0; i < total_number_of_mailboxes; i++)
    {
        if (semctl(semid, i, SETVAL, 1) == -1)
        {
            cout << "ERRNO:" << errno << endl;
			printf("Mailbox semaphore initialization error!\n");
            exit(0);
        }
    }

    //system_v信号量函数,创建一个信号量集，用于控制邮箱的读写操作
    int semid2 = semget(IPC_PRIVATE, total_number_of_mailboxes, IPC_CREAT | 0666);
    if (semid2 == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Mailbox semaphore set creation error!\n");
        exit(0);
    }
    for (int i=0; i < total_number_of_mailboxes; i++)
    {
        if (semctl(semid2, i, SETVAL, 1) == -1)
        {
            cout << "ERRNO:" << errno << endl;
			printf("Mailbox semaphore initialization error!\n");
            exit(0);
        }
    }

    int semid3 = semget(IPC_PRIVATE, total_number_of_mailboxes, IPC_CREAT | 0666);
    if (semid3 == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Mailbox semaphore set creation error!\n");
        exit(0);
    }
    for (int i=0; i < total_number_of_mailboxes; i++)
    {
        if (semctl(semid3, i, SETVAL, 1) == -1)
        {
            cout << "ERRNO:" << errno << endl;
			printf("Mailbox semaphore initialization error!\n");
            exit(0);
        }
    }

    int semid4 = semget(IPC_PRIVATE, total_number_of_mailboxes, IPC_CREAT | 0666);
    if (semid4 == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Mailbox semaphore set creation error!\n");
        exit(0);
    }
    for (int i=0; i < total_number_of_mailboxes; i++)
    {
        if (semctl(semid4, i, SETVAL, 1) == -1)
        {
            cout << "ERRNO:" << errno << endl;
			printf("Mailbox semaphore initialization error!\n");
            exit(0);
        }
    }
	
    int semid5= semget(IPC_PRIVATE, total_number_of_mailboxes, IPC_CREAT | 0666);
    if (semid5 == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Mailbox semaphore set creation error!\n");
        exit(0);
    }
    //对第i个信号量进行SETVAL操作
    for (int i=0; i < total_number_of_mailboxes; i++)
    {
        if (semctl(semid5, i, SETVAL, 1) == -1)
        {
            cout << "ERRNO:" << errno << endl;
			printf("Mailbox semaphore initialization error!\n");
            exit(0);
        }
    }	

    //创建一个信号量，来控制隧道里的车辆数量
    int semid1 = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semid1 == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Tunnel semaphore creation error!\n");
        exit(0);
    }
	//对信号量进行SETVAL操作，值是隧道车的最大数量
    if (semctl(semid1, 0, SETVAL, maximum_number_of_cars_in_tunnel) == -1)
    {
        cout << "ERRNO:" << errno << endl;
		printf("Tunnel semaphore initialization error!\n");
        exit(0);
    }
    //创建并初始化共享内存区；
    int size = total_number_of_mailboxes*memory_segment_size * sizeof(char);
    int shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    //建立新共享内存对象
    if (shmid == -1)
    {
        cout << "ERRNO:" << errno << endl;
        printf("Shared memory creation failed!\n");
        exit(0);
    }
    void * addr = shmat(shmid, NULL, 0);	//将共享内存区域映射到进程的虚拟地址空间
    tunnel* T = new tunnel(semid, semid2,semid3,semid4,semid5,addr);
	//printf("11111");

    //创建子进程，每个子进程代表一辆车
    pid_t id;
    for (int i = 0; i < total_number_of_cars; i++)
    {
        if(!input_car(file, buf))//如果出错，删除所有东西
        {
			printf("Input error!\n");
            semctl(semid1, 0, IPC_RMID);
            semctl(semid, 0, IPC_RMID);
            semctl(semid2, 0, IPC_RMID);
            semctl(semid3, 0, IPC_RMID);
            shmctl(shmid, IPC_RMID, NULL);
            return 0;
        }
        id = fork();
        if (id == -1) //进程创建失败
        {
            cout << "ERRNO:" << errno << endl;
            cout << "Child process" << i + 1 << "failed to create!" << endl;
            semctl(semid1, 0, IPC_RMID);
            semctl(semid, 0, IPC_RMID);
            semctl(semid2, 0, IPC_RMID);
            semctl(semid3, 0, IPC_RMID);
            shmctl(shmid, IPC_RMID, NULL);
            system("PAUSE");
            return 0;
        }
        else if (id == 0) //子进程
        {
            cout<<"Car "<<i+1<<" is running."<<endl;
            break;
        }
        buf.str("");//初始化缓冲区
    }
    if(id == 0)  	//子进程
    {
        deal_car(T, buf, semid1);
        return 0;
    }
    else//主进程
    {
        while(semctl(semid1, 0, GETVAL)<maximum_number_of_cars_in_tunnel)
        {
            usleep(100);
        }
		printf("All cars have passed through the tunnel.\n");
        semctl(semid1, 0, IPC_RMID);
        semctl(semid, 0, IPC_RMID);
        semctl(semid2, 0, IPC_RMID);
        semctl(semid3, 0, IPC_RMID);
        shmctl(shmid, IPC_RMID, NULL);
        return 0;
    }
}


