#ifdef  __TRANFER_H__
#define __TRANFER_H__

struct TParam
{
	int in;
	int out; 
	pthread_t bro;//两个线程互相照应
};
#endif