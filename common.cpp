#include "common.h"

int recv_n(int sock,char *buf,int len)
//一直读，如果不到len长度就阻塞式的读
{
	char cur = buf;
	int left = len;
	while(left > 0)
	{
		int ret = recv(sock,buf,left,0);
		if(left <= 0)
		{
			return -1;
		}
		cur += ret;
		left-= ret;
	}
	return 0;
}

int send_n(int sock,const char* buf,int len)
{
	const char* cur = buf;
	int left = len;
	while(left > 0)
	{
		int ret = send(sock,cur,left,0);
		if(ret < 0)
		{
			return -1;
		}
		cur += ret;
		left -= ret;
	}
	return 0;
}