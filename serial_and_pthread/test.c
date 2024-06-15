#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "serialport.h"
int     fd;
void *mypthreadFuncRecv(void *pvoid)
{
    char    rbuf[128];                               //存信息的数组	
	while(1)
	{
 		serialport_receive(fd,rbuf,sizeof(rbuf),100);    // 接收信息			
	}	
}
int main(int argc,char **argv)
{

        char    port[]="/dev/pts/1";
        fd=serialport_open(port);
        serialport_set(fd,115200,0,8,1,0);  
        pthread_t recv_pthd;
        pthread_create(&recv_pthd,NULL,mypthreadFuncRecv,NULL);  
        while(1)
        {
        	sleep(1);
        }
        return 0;
}