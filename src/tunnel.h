
#include"syn.h"

//隧道类
class tunnel
{
	int wsid;//信号量
	int rsid;
	int xsid;
	int zsid;
	int ysid;
	vector<int> wcount;;//写者计数
	vector<int> rcount;//读者计数
	vector<char*> mail;//邮箱集合
public:
	tunnel(int s,int s1,int s2,int s3,int s4, void *addr)
	{
		wsid = s;
		rsid=s1;
		xsid=s2;
		zsid=s3;
		ysid=s4;
		wcount.resize(total_number_of_mailboxes);
		rcount.resize(total_number_of_mailboxes);
		for (int i= 0; i < total_number_of_mailboxes; i++)
		{
			rcount[i]=0;
			wcount[i]=0;
		}
		char *c = (char*)addr;
		for (int i= 0; i < total_number_of_mailboxes; i++)
		{
			int j = i*memory_segment_size;
			char * m1 = c + j;
			mail.push_back(m1);
		}
	}
	bool tunnel_Set(int s,int s1,int s2,int s3,int s4);
	//邮箱写入no号邮箱，s是写入的字符串，n是字符串长度，t是花费时间
	bool write(int no, char *s, int n, int t,int &flag);
	//邮箱从no号邮箱读出，s是读出的字符串,n是字符串长度,m是维护的读指针，t是花费时间
	bool read(int no, char* &s, int n, int &m,int t,int &flag);

};