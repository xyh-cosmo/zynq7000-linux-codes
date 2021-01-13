#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/mman.h>
 
int main()
{
	int fd = open("/dev/mem", O_RDWR | O_SYNC);
	volatile unsigned int* led_base = mmap(NULL, 65535, PROT_READ | PROT_WRITE,
    		MAP_SHARED, fd, 0x43C00000);
	while(1)
	{
		memset((void *)(led_base+0), 1, 1);
		sleep(1);
		memset((void *)(led_base+0), 0, 1);
		sleep(1);
		memset((void *)(led_base+1), 1, 1);
		sleep(1);
		memset((void *)(led_base+1), 0, 1);
		sleep(1);
		memset((void *)(led_base+2), 1, 1);
		sleep(1);
		memset((void *)(led_base+2), 0, 1);
		sleep(1);
		memset((void *)(led_base+3), 1, 1);
		sleep(1);
		memset((void *)(led_base+3), 0, 1);
		sleep(1);
	}
	
	return 0;
}
