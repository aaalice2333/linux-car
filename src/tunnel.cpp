#include"tunnel.h"

bool tunnel::read(int no, char* &s, int n,int &m, int t,int &flag)
{
    if (no > total_number_of_mailboxes)
    {
		printf("Mailbox %d is illeagal.\n",no);
        return 0;
    }

   // wait(zsid,no-1);
	//wait(rsid,no-1);
    //wait(xsid,no-1);
    //rcount[no-1]++;
    //if(rcount[no-1]==1)
     //   wait(wsid, no - 1);
    //signal(xsid,no-1);
    //signal(rsid,no-1);
	//signal(zsid,no-1);

    int len=strlen(mail[no-1]);
    //printf("r: %d\n",len);
    if (m >= len)
    {
        usleep(t);
        return 0;
    }
    int i;
    for (i = m; i < len&&i<m+n; i++)
    {
        s[i-m] = mail[no-1][i];
    }
    //printf("r :%s\n",s);
    if (i == len)//如果已经读到末尾了
    {
        //strcat(s,"END");
        //m = i;
		flag=1;
    }
    m = i;
    usleep(t);
	
    //wait(xsid,no-1);
    //rcount[no-1]--;
    //if(rcount[no-1]==0)
      //  signal(wsid, no - 1);
    //signal(xsid, no - 1);
    return 1;
}


bool tunnel::write(int no, char *s, int n, int t,int &flag)
{
    if (no > total_number_of_mailboxes)
    {
        cout << "mailbox " << no << " is illegal" << endl;
        return 0;
    }

    //wait(ysid,no-1);
	//wcount[no-1]++;
    //if(wcount[no-1]==1)
    //    wait(rsid,no-1);
    //signal(ysid,no-1);
    wait(wsid,no-1);

    int len=strlen(mail[no-1]);
    //printf("w: %d\n",len);
    if (len >= memory_segment_size)
    {
        usleep(t);
        signal(wsid, no - 1);//也不知道用什么信号量
        return 0;
    }
    int i;
    char c;
    for (i = len; i < len+n&&i<memory_segment_size; i++)
    {
        c= s[i-len];
        mail[no-1][i]=c;
    }
		if(i>=memory_segment_size)
		flag=1;
    mail[no-1][i]='\0';
    //printf("w: %d\n",len);
    usleep(t);

    signal(wsid,no-1);
    //wait(ysid,no-1);
    //wcount[no-1]--;
    //if(wcount[no-1]==0)
    //    signal(rsid,no-1);
    //signal(ysid,no-1);

    return 1;
}