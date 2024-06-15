#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
 
#include "serialport.h"
 
//打开串口
int serialport_open(char *port)    //参数port是要打开的串口设备        
{
        int     fd;
 
        fd=open(port,O_RDWR|O_NOCTTY|O_NDELAY);
        if(fd<0)
        {
                printf("can't open serial port\n");
                return -1;
        }
        printf("open serial port '%s' successfully and fd=%d\n",port,fd);
 
        return fd;
}
 
//设置波特率、流控制、数据位、停止位、校检位
int serialport_set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int checkbits)    
{
        struct termios  options;
        int             i;
        int             speed_arr[] = { B115200, B19200, B9600, B4800, B2400, B1200, B300};
        int             name_arr[] = {115200,  19200,  9600,  4800,  2400,  1200,  300};
 
        if(tcgetattr(fd,&options)!=0)
        {
                printf("tcgetattr() failure:%s\n",strerror(errno));
                return -1;
        }
        printf("tcgetattr() successfully!\n");
 
        //设置输入输出波特率
        for ( i= 0;  i < sizeof(speed_arr) / sizeof(int);  i++)
        {
                if  (speed == name_arr[i])
                {
                        cfsetispeed(&options, speed_arr[i]);
                        cfsetospeed(&options, speed_arr[i]);
                }
        }
 
        //不会占用串口，能读取输入数据
        options.c_cflag |= CLOCAL|CREAD;
 
        switch(flow_ctrl)
        {
                case 0:         //不使用流控制
                        options.c_cflag &=~ CRTSCTS;
                        break;
 
                case 1:         //使用硬件流控制
                        options.c_cflag |= CRTSCTS;
                        break;
 
                case 2:         //使用软件流控制
                        options.c_cflag |= IXON | IXOFF | IXANY;
                        break;
        }
 
        //设置数据位
        options.c_cflag &=~ CSIZE;
        switch(databits)
        {
                case 7:
                        options.c_cflag |= CS7;
                        break;
 
                case 8:
                        options.c_cflag |= CS8;
                        break;
        }
 
        //设置停止位
        switch(stopbits)
        {
                case 1:
                        options.c_cflag &=~ CSTOPB;
                        break;
 
                case 2:
                        options.c_cflag |= CSTOPB;
                        break;
        }
 
        //设置校检位
        switch(checkbits)
        {
                case 0: //无校验位
                        options.c_cflag &=~ PARENB;
                        options.c_iflag &=~ INPCK;
                        break;
 
                case 1: //奇
                        options.c_cflag |= PARENB;
                        options.c_cflag |= PARODD;
                        options.c_iflag |= (INPCK|ISTRIP);//inpck打开奇偶校验,istrip去掉字符第八位
                        break;
 
                case 2: //偶
                        options.c_cflag |= PARENB;
                        options.c_cflag |= ~PARODD;
                        options.c_cflag |= (INPCK|ISTRIP);
                        break;
        }
 
        //设置等待时间和最小接收字符
        options.c_cc[VTIME]=0;
        options.c_cc[VMIN]=0;
 
        tcflush(fd,TCIFLUSH);   //如果发生数据溢出，接收数据，但是不再读取。刷新收到的数据但是不读
 
        if(tcsetattr(fd,TCSANOW,&options)!=0)
        {
                printf("tcsetattr() failure:%s\n",strerror(errno));
                return -1;
        }
        printf("tcsetattr() successfully!\n");
 
        return 0;
}
 
//读信息
int serialport_receive(int fd,char *rbuf,int rbuf_len,int timeout)
{
        int             rv=-1;
        int             len=-1;
        fd_set          rd_set;
        struct timeval  time_out;
 
        if(!rbuf||rbuf_len<=0)
        {
                printf("receive func get invalid parameter\n");
                return -1;
        }
 
        if(timeout)
        {
                time_out.tv_sec=(time_t)timeout;
                time_out.tv_usec=0;
 
                FD_ZERO(&rd_set);
                FD_SET(fd,&rd_set);
 
                rv=select(fd+1,&rd_set,NULL,NULL,&time_out);
                if(rv<0)
                {
                        printf("select failure:%s\n",strerror(errno));
                        return -1;
                }
                else if(rv==0)
                {
                        printf("select timeout\n");
                        return -1;
                }
 
        }
 
        if(FD_ISSET(fd,&rd_set))
        {
                memset(rbuf,0,sizeof(rbuf));
                if((len=read(fd,rbuf,rbuf_len))<0)
                {
                        printf("read from serial port failure:%s\n",strerror(errno));
                        return -1;
                }
                printf("read %d bytes data from serial port fd=%d:'%s'\n",len,fd,rbuf);
        }
 
        return 0;
}
 
//发信息
int serialport_send(int fd,char *send,int data_len)
{
        int len=-1;
 
        len=write(fd,send,data_len);
        if(len<0)
        {
                printf("write failure:%s\n",strerror(errno));
                return -1;
        }
        //printf("write to serial port successfully!\n");
        printf("write %d bytes data to serial port fd=%d:'%s'\n",len,fd,send);

        return 0;
}
 
//关闭串口
void serialport_close(int fd)
{
        close(fd);
}
