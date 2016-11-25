#include <iostream>
using namespace std;
int main(int argc,char*argv[])
{
	if(argc != 2)
	{
		usage(argv[0]);
		exit(1);
	}
	int lsnr = socket(AF_INET,SOCK_STREAM,0);
	assert(lsnr != -1);
	struct sockaddr_in laddr;
	memset(&laddr,0,sizeof(laddr));
	laddr.sin_family = AF_INET;
	laddr.sin_addr.s_addr = INADDR_ANY;
	laddr.sin_port = htons(atoi(argv[1]));
	assert(0 == bind(lsnr,(struct sockaddr*)&laddr,sizeof(laddr)));
	assert(0 == listen(lsnr,5));
	
	while(1)
	{
		struct sockaddr_in caddr;
		int len = sizeof(caddr);
		int in = accept(lsnr,(struct sockaddr*)&caddr,&len);
		
		pid_t pid = fork();
		//fork两次解决僵尸进程问题
		assert(pid >= 0);
		if(pid > 0)//father
		{
			waitpid(pid,NULL,0);//等子进程退出
		}else{
			pid_t pid2 = fork();
			assert(pid2 >= 0);
			
			if(pid2 > 0)
			{
				exit(0);//退出,被爷爷进程回收
			} 
			else //child变为孤儿进程
			{
				close(lsnr);
				return proc_socks(in);
			}
		}
	}
}

int proc_socks(int in)
{
	SKReq sreq;//协议
	assert(0 == recv_n(in,&sreq,sizeof(sreq)));
	//收协议中的内容
	assert(sreq.ver == 5 && sreq.n == 1);
	//只有一种选择
	char methods[8];
	assert(0 == recv_n(in,methods,));
	//收第二块协议
	assert(0 == method[0]);
	//选择为0
	SKRep rep;
	rep.ver = 5;
	rep.m = 0;
	assert(0 == send_n(in,(char*)&rep,sizeof(rep)));
	//以上协商完成，下一步接收地址
	//
	
	sockaddr_in out_addr;
	memset(&,0,sizeof);
	out_addr.
	//初始化
	
	
	
	AddrReq areq;
	assert(0 == recv_n(in ,(char*)areq,sizeof(areq)));
	//CMD认为一定是1且不处理
	assert(areq.ver == 5 && areq.cmd == 1 && areq.rsv == 0);
	if(areq.atype == 1)//IPV4
	{
		//可以得到地址
		assert(0 == recv_n(in,\
							(char*)&out_addr.sin_addr.s_addr,\
							sizeof(out_addr.sin_addr.s_addr)));
		
	}
	else if(areq.atype == 3)// 域名
	{
		//得到域名，间接得到地址
		//第一个字节为域名长度，后面为变长的内容
		char dlen;//字节序
		assert(0 == recv_n(in,(char*)&dlen,1));
		char domain[256];
		assert(0 == recv_n(in,domain,dlen));
		domain[dlen] = 0;
		hostent *host = gethostbyname(domain);//输入参数为域名，返回结构体hostent,重视其中的char**h_addr_list
		assert(host && host->h_addrtypr = AF_INET && host->length > 0);
		memccpy(&out_addr.sin_addr.s_addr,host->h_addr_list[0],sizeof(out_addr.sin_addr.s_addr));
		cout<<"domain"<<endl;
	}
	else
	{
		assert(0);
	}
	//取端口
	short port;
	assert(0 == recv_n(in,(char*)&out_addr.sin_port,sizeof(out_addr.sin_port)));
	int out = socket(AF_INET,SOCK_STREAM,0);
	assert(out != -1);
	//连接
	assert(0 == connect(out,(sockaddr*)&out_addr,sizeof(out_addr)));
	
	//返回reply
	AddrRep arep;
	arep.ver = 5;
	arep.cmd = 0;
	arep.rsv = 0;
	arep.atype = 1;
	sockaddr_in local_addr;
	socket_t slen = sizeof(local_addr);
	assert(0 == getsockname(out,(sockaddr*)local_addr,&slen));
	memccpy(&arep.addr,local_addr.sin_addr.s_addr,sizeof(local_addr.sin_addr.s_addr));
	memccpy(&arep.port,&local_addr.sin_port,sizeof(short));
	assert(send_n(in,(char*)&arep,sizeof(arep)) == 0);
	
	//传输
	//起两个线程，一个做上行，一个做下行
	TParam up = {in,out,0};
	TParam down = {out,in,0};
	assert(0 == pthread_create(&down.bro,NULL,transfer,&up));
	assert(0 == pthread_create(&up.bro,NULL,transfer,&down));
	pthread_join(up.bro,NULL);
	pthread_join(down.bro,NULL);
	return 0;
}