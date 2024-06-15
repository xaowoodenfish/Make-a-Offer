#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_
 
extern int serialport_open(char *port);
extern int serialport_set(int fd,int speed,int flow_ctrl,int databits,int stopbits,int checkbits);
extern int serialport_receive(int fd,char *rbuf,int rbuf_len,int timeout);
extern int serialport_send(int fd,char *send,int data_len);
extern void serialport_close(int fd);
 
#endif
