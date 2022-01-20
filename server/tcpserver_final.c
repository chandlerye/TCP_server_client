// 程序说明：设置39行的端口，编译运行，服务端就设置好了
//编译指令gcc tcpserver_final.c -lpthread -o serverfinal

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

char buff[1024];		// 通信接受数据缓存
char name_save[1024];	//客户端昵称缓存
char maohao[1] = {':'}; //冒号

struct SockInfo //信息结构体
{
	struct sockaddr_in addr; //ip和端口的结构体
	int fd;					 //文件描述符
	int len;				 //接收数据返回的标识符,大于0接收到数据，等于0客户端关闭，小于0出问题
	char client_name[1024];	 //客户端设置的昵称存储
};

struct SockInfo infos[512]; //客户端对象

void *working(void *arg);			  //客户端
void sendall(struct SockInfo *pinfo); //服务端发送消息函数

int main()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0); //创建监听套接字
	if (fd == -1)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in saddr; //存放ip,端口的结构体

	saddr.sin_family = AF_INET;			//ipv4 ipv6 都行
	saddr.sin_port = htons(7777);		//端口,网络字节序
	saddr.sin_addr.s_addr = INADDR_ANY; //当服务器有多个网卡时,这样设置可以连到任意网卡

	int ret = bind(fd, (struct sockaddr *)&saddr, sizeof(saddr)); //将监听套接字和服务器ip,端口绑定
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	ret = listen(fd, 128);
	if (ret == -1)
	{
		perror("listen");
		return -1;
	}

	//初始化结构体数组
	int max = sizeof(infos) / sizeof(infos[0]);
	int i;
	for (i = 0; i < max; ++i)
	{
		bzero(&infos[i], sizeof(infos[i]));
		infos[i].fd = -1;
	}

	int addrlen = sizeof(struct sockaddr_in);

	while (1) //等待客户端连接循环
	{
		struct SockInfo *pinfo;
		int j;
		for (j = 0; j < max; ++j)
		{						   //判断空闲区域
			if (infos[j].fd == -1) //fd为-1说明未使用
			{
				pinfo = &infos[j]; //pinfo指向空闲区域
				break;			   //找到空闲区域就推出
			}
		}

		int cfd = accept(fd, (struct sockaddr *)&pinfo->addr, &addrlen); //有个新的连接那么accept就会返回新的cfd
		pinfo->fd = cfd;

		if (cfd == -1)
		{
			perror("accept");
			break; //或者continue
			return -1;
		}

		//创建子线程
		pthread_t tid;
		pthread_create(&tid, NULL, working, pinfo);
		pthread_detach(tid);
	}

	close(fd);
	return 0;
}

//子函数
void *working(void *arg)
{
	struct SockInfo *pinfo = (struct SockInfo *)arg;
	char ip[24];

	printf("客户端ip:%s , 端口port:%d\n",
		   inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
		   ntohs(pinfo->addr.sin_port));

	recv(pinfo->fd, pinfo->client_name, sizeof(pinfo->client_name), 0); //接受客户端返回的客户端昵称

	while (1)
	{
		pinfo->len = recv(pinfo->fd, buff, sizeof(buff), 0); //读写都需要文件描述符
		if (pinfo->len > 0)
		{
			printf("%s say:%s\n", pinfo->client_name, buff); //服务端显示
			sendall(pinfo);									 //给所有客户端发送消息
		}
		else if (pinfo->len == 0)
		{
			printf("client close\n");
			printf("客户端ip:%s , 端口port:%d 断开连接\n",
				   inet_ntop(AF_INET, &pinfo->addr.sin_addr.s_addr, ip, sizeof(ip)),
				   ntohs(pinfo->addr.sin_port));
			break;
		}
		else
		{
			perror("recv");
			break;
		}

		memset(buff, 0, sizeof(buff));
	}

	close(pinfo->fd);
	pinfo->fd = -1;
	memset(pinfo->client_name, 0, sizeof(pinfo->client_name));

	return NULL;
}

void sendall(struct SockInfo *pinfo)
{
	int j;
	for (j = 0; j < sizeof(infos) / sizeof(infos[0]); ++j)
	{ //判断空闲区域
		if (infos[j].fd != -1)
		{
			strcpy(name_save, pinfo->client_name);
			strcat(name_save, maohao); //将冒号相连
			strcat(name_save, buff);
			send(infos[j].fd, name_save, strlen(name_save), 0); //strlen是数组有多大就读多大
		}
	}
}
