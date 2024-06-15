#include "serialport.h"
 
int main(int argc,char **argv)
{
        int     fd;
        char    port[]="/dev/pts/1";
        char    rbuf[128];                               //存信息的数组
 
        fd=serialport_open(port);
 
        serialport_set(fd,115200,0,8,1,0);    
 
        serialport_receive(fd,rbuf,sizeof(rbuf),100);    // 接收信息
 
        return 0;
}
