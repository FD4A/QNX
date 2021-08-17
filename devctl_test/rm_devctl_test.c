#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "rm_train_api.h"

int main(int argc, char *argv[])
{
	if( 4!=argc )
	{
		printf("argv[1] - device path\n");
		printf("argv[2] - hex command num\n");
		printf("argv[3] - int value to send\n");
		printf("Example:\n");
		printf("    ./rm_devctl_test 0x1 14\n");
		return EXIT_FAILURE;
	}

	int fd = open(argv[1],O_RDWR);
	if(-1==fd)
	{
		printf("fail open %s\n",argv[1]);
		return EXIT_FAILURE;
	}

	int cmd = atoi(argv[2]);
	cmd = __DIOT(_DCMD_MISC,cmd,int);

	int value = atoi(argv[3]);
	int ans = devctl(fd,cmd,&value,sizeof(value),NULL);
	printf("devctl, error = %d(%s)\n",ans,strerror(ans));
	return 0;
}
