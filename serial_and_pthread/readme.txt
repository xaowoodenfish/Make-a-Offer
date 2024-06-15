1:run as python3 mkptych.py&
2:gcc test.c serialport.c -pthread -o test
3:gcc write.c serialport.c -o write
4:gcc read.c  serialport.c -o read