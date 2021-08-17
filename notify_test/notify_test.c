#include "NotifyTest.h"

static int parsefunc(select_context_t *ctp, int fd,  unsigned int flags, void* arg);
static int source_disconnect(message_context_t *ctp, int code, unsigned flags, void *handle);

int main(int argc, char *argv[])
{
	/* open fd */
	if( 2!=argc )
	{
		printf("argv[1] - set device path name, from which you want "
				"receive notifications.\n");
		return -1;
	}

	dispatch_t *dpp;
	if( (dpp=dispatch_create())==NULL )
	{
		printf("fail dispatch_create()\n");
		return -1;
	}
	printf("success dispatch_create()...\n");

	int fd;
	char *path = argv[1];
	fd=open(path,O_RDONLY);
	if(fd==-1)
	{
		printf("fail open [%s]\n",path);
		return -1;
	}
	printf("success open [%s]...\n",path);

	/* look for notifications */
	int res = select_attach( dpp, NULL, fd, SELECT_FLAG_READ | SELECT_FLAG_REARM, parsefunc, NULL);
	if( -1==res )
	{
		printf("fail select_attach\n");
		return -1;
	}

	/* look for disconnections */
	if( -1==pulse_attach(dpp, 0, _PULSE_CODE_COIDDEATH, source_disconnect, NULL ))
	{
		printf("fail pulse_attach()..\n");
		return -1;
	}
	printf("error pulse_attach()..\n");

	dispatch_context_t *ctp;
	ctp = dispatch_context_alloc(dpp);
	while(1)
	{
		if((ctp = dispatch_block(ctp)) == NULL)
			{return EXIT_FAILURE;}
		dispatch_handler(ctp);
	}
	return EXIT_SUCCESS;
}

static int parsefunc(select_context_t *ctp, int fd,  unsigned int flags, void* arg)
{
	static char rotator[]={"-\\|/"};
	static int i=0;
	printf("Notify is ok %c\n",rotator[i]);
	i++;
	if(i>3)
		{i=0;}
	return 0;
}

static int source_disconnect(message_context_t *ctp, int code, unsigned flags, void *handle)
 {
	 printf("observable source is closed\n");
	 exit(0);
 }
