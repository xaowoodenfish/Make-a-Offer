#include "serialport.h"                           // 引入头文件   
 
int main(int argc,char **argv)
{
        int     fd;                               // 文件描述符
        char    port[]="/dev/pts/2";            // 能收发信息的串口
        char    send[]="send msg ";                   // 要发的信息
 
        fd=serialport_open(port);                 // 打开串口
 
        serialport_set(fd,115200,0,8,1,0);        // 设置串口
 
        serialport_send(fd,send,sizeof(send));    // 发送信息

        return 0;
}
