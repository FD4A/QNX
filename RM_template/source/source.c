#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "source.h"

/* this function execute in dispatcher thread */
int Source_Init(Source_t *source, char *path, unsigned connect_timeout)
{
	pthread_mutex_init(&source->access.mutex,NULL);
	pthread_cond_init(&source->access.maywork,NULL);
	source->access.data_ready = 0;
	source->access.acces_flag = 0;
	source->access.fd = -1;
	source->access.connect_timeout = connect_timeout;
	strncpy(source->access.path,path,SOURCE_PATH_MAX_STR_LEN);
	source->access.path[SOURCE_PATH_MAX_STR_LEN]='\0';
	return 0;
}

/* this function execute in dispatcher thread */
int Source_StartRoutine(Source_t *source)
{
	if( EOK!=pthread_create(NULL,NULL,Source_Routine,(void*)source))
	{
		return -1;
	}
	return 0;
}

/* this function execute in dispatcher thread */
void Source_AccesFlagDownAndNotify(Source_t *source)
{
	 pthread_mutex_lock(&source->access.mutex);
	 source->access.acces_flag = 0;
	 pthread_cond_signal(&source->access.maywork);
	 pthread_mutex_unlock(&source->access.mutex);
}

static int Source_Open(SourceAccess_t *access);
static int Source_Work(Source_t *source);
static int Source_Close(SourceAccess_t *access);
/* this is Routine thread */
void *Source_Routine(void *arg)
{
	Source_t *source = (Source_t*)arg;
	while(1)
	{
		/*open and action on source open*/
		if ( 0==Source_Open(&source->access) )
		{
			/* lock while 1==acces_flag && 0==data_ready,
			 * if 0==acces_flag return, if 1==data_ready
			 * data_ready=0 and take data from source
			 * acces_flag changed in Source_Open or in
			 * rm_source_disconnect()*/
			Source_Work(source);
		}
		Source_Close(&source->access);
	}
	return NULL;/*noreturn*/
}

/* this function execute in Routine thread */
static int Source_Open(SourceAccess_t *access)
{
	if( -1==(access->fd = open(access->path,O_RDONLY)) )
	{
		return -1;
	}

	pthread_mutex_lock(&access->mutex);
	access->acces_flag = 1;
	pthread_mutex_unlock(&access->mutex);
	return 0;
}

/* this function execute in Routine thread */
static int Source_Work(Source_t *source)
{
	pthread_mutex_lock(&source->access.mutex);
	while( 1==source->access.acces_flag && 0==source->access.data_ready)
	{
		pthread_cond_wait(&source->access.maywork,&source->access.mutex);
	}
	source->access.data_ready= 0;
	pthread_mutex_unlock(&source->access.mutex);
	if( 0==source->access.acces_flag )
		{return 0;}/*move to close*/
	/*read data*/
	return 0;
}

/* this function execute in Routine thread */
static int Source_Close(SourceAccess_t *access)
{
	close(access->fd);
	access->fd= -1;
	sleep(access->connect_timeout);
	return 0;
}
