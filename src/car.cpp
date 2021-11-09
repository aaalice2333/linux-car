
#include"car.h"

struct timeval now;
struct timeval end1;


bool car::overtime(int t)
{
    if (time+t > tunnel_travel_time)//
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

bool car::car_into_tunnel(int &usetime)
{
    if (state != 1)
    {
        printf("state error!\n");
        return 0;
    }
    printf("Car %d is outside the tunnel and prepares to get in.\n",no);

    wait(sid, 0);

    long long begintime;
    long long currenttime;


    gettimeofday(&now, NULL);
    begintime=int(1000 *(now.tv_sec) + now.tv_usec);
    //printf("Runing time:%lld ms\n", begintime);

    printf("Car %d is getting into the tunnel.\n",no);

    gettimeofday(&end1, NULL);
    currenttime=int(1000 *(end1.tv_sec) + end1.tv_usec);
    //printf("Runing time:%lld ms\n", currenttime);

    usetime=currenttime-begintime;
    printf("car %d spent %d ms getiing into the tunnel.\n", no,usetime);
	
	time +=usetime;

    state = 2;

    return 1;
}

bool car::car_out_tunnel()
{
    if (state != 2)
    {
        printf("state error!\n");
        return 0;
    }
    usleep(tunnel_travel_time-time);

    state = 3;

    long long begintime;
    long long currenttime;
    int usetime;
    gettimeofday(&now, NULL);
    begintime=int(1000 *(now.tv_sec) + now.tv_usec);
    //printf("Runing time:%lld ms\n", begintime);

    //cout<<str.length()<<endl;

    if(str.length()!=1)

        printf("Car %d passed the tunnel. At this time, the memory of car is%s .\n", no,str.c_str());

    else

        printf("Car %d passed the tunnel. At this time, the memory of car is NULL.\n", no);

    gettimeofday(&end1, NULL);
    currenttime=int(1000 *(end1.tv_sec) + end1.tv_usec);
    //printf("Runing time:%lld ms\n", currenttime);

    usetime=currenttime-begintime;
    printf("car %d spent %d ms getiing out of the tunnel.\n", no,usetime);

    signal(sid, 0);
    //printf("signal\n");
    return 1;
}

bool car::car_write(tunnel *T,int num,char *s,int n,int t)
{
	int flag=0;
    if (state == 3)
    {
        printf("car %d is outside the tunnel writing.\n", no);
        return 0;
    }
    if (T->write(num, s, n, t,flag))
    {
        printf("Car %d uses %d ms to write string \"%s\" to mailbox %d\n", no,t,s,num);
		if(flag==1)
		   printf("mailbox %d is full\n", num);
        time += t;//计算通过隧道的时间
        return 1;
    }
    else
    {
        printf("car %d failed to write to the mailbox %d.\n", no,num);
        return 0;
    }
}

bool car::car_read(tunnel *T, int num, char* &s, int n, int t)
{
	int flag=0;
    if (state == 3)
    {
        printf("car %d is outside the tunnel reading.\n", no);
        return 0;
    }
    if (T->read(num, s, n, m[num-1], t,flag))
    {
        //printf("11111111111111111111111\n%s",s);
        printf("Car %d uses %d ms to read string \"%s\" from mailbox %d.\n", no,t,s,num);
		if(flag==1)
		   printf("This is the end of mailbox %d.\n", num);
        string s1=s;
        str=str+s1;
        time += t;//计算通过隧道的时间
        return 1;
    }
    else
    {
        printf("car %d had already read to the end of the mailbox %d.\n", no,num);
        return 0;
    }
}
