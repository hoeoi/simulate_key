#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <stdlib.h>
#include <pthread.h>  
#include <linux/input.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <stdint.h>

void simulate_key(int fd, int key, int value)

{

	struct input_event event;
	event.type = EV_KEY;

	event.code = key;//要模拟成什么按键

	event.value = value;//是按下还是释放按键或者重复

	gettimeofday(&event.time,0);

	//printf("event size is %d\n",sizeof(event));
	//char *byte,i;
	//byte = &event;
	//for(i = 0; i < sizeof(event); i++)
	//{
	//	printf("%02x ",byte[i]);
	//}
	//printf("\n");
	if(write(fd,&event,sizeof(event)) < 0){

		printf("simulate key error~~~\n");

		return ;

	}
    	event.type = EV_SYN;
	event.code = SYN_REPORT;
	event.value = 0;
	if(write(fd,&event,sizeof(event)) < 0){

		printf("simulate key error~~~\n");

		return ;

	}
    event.type = EV_SYN;

}

int main(int argc , char *argv[])
{
	int eventfd;
	if(argc != 4)
	{
		printf("help:%s <event> <key> <value>\n",argv[0]);
		printf("     key: 0-9 [KEY_0 - KEY_9]\n");
		printf("     value: 0/1, key down is 1, key up is 0\n");
		printf("example for KEY_1 press down : %s /dev/input/event0 1 1\n",argv[0]);
		return -1;
	}
	
	
	eventfd = open(argv[1],O_RDWR);
	if(eventfd < 0)
	{
		printf("open file error\n");
		return -1;
	}
	int argv2 = atoi(argv[2]);
/*#define KEY_1			2
#define KEY_2			3
#define KEY_3			4
#define KEY_4			5
#define KEY_5			6
#define KEY_6			7
#define KEY_7			8
#define KEY_8			9
#define KEY_9			10
#define KEY_0			11*/
	int key = (argv2 == 0 ? 11 : argv2+1);  
	int value = atoi(argv[3]);
	simulate_key(eventfd, key, value);
	
	
}
