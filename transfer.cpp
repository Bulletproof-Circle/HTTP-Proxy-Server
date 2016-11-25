//线程

#include <pthread.h>


const int BUF_LEN = 1048576;
void *transfer(void*p)//做盲转发
{
	TParam *param = (TParam*)p;
	int in = param->in;
	int out = param->out;
	int bro = param->bro;
	char buf=[BUF_LEN];
	while(1)
	{
		//从in到out一个方向上的传输
		int ret = recv(in,buf,BUF_LEN,0);
		//收不管收多少都收
		if(ret <= 0)
		{
			break;
		}
		//盲转发
		ret = send_n(out,buf,ret);
		if(ret < 0)
			break;
	}
	close(in);
	close(out);
	pthread_cancle(bro);
	return NULL;
}