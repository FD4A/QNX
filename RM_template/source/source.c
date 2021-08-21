#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include "source.h"

#define DFLINE printf("%s(%d)\n",__FUNCTION__,__LINE__);

/* I want have one dataReady wake up flag function */
typedef struct
{
	int ind;
	SourceAccess_t *access;
}forDataReadyFunc_t;
static forDataReadyFunc_t forDataReadyFunc[SOURCE_DATA_READY_COUNT];

/* this function execute in dispatcher thread */
int Source_Init(Source_t *source, dispatch_t *dpp, char *path[], unsigned connect_timeout)
{
	source->access.dpp = dpp;
	pthread_mutex_init(&source->access.mutex,NULL);
	pthread_cond_init(&source->access.maywork,NULL);
	source->access.acces_flag = 0;
	source->access.connect_timeout = connect_timeout;
	source->access.way = 0;

	int i;
	for(i=0;i<SOURCE_DATA_READY_COUNT;++i)
	{
		source->access.fd[i] = -1;
		strncpy(&source->access.path[i][0],&path[i][0],SOURCE_PATH_MAX_STR_LEN);
		source->access.path[i][SOURCE_PATH_MAX_STR_LEN]='\0';
		source->access.data_ready[i] = 0;
		forDataReadyFunc[i].ind = i;
		forDataReadyFunc[i].access = &source->access;
	}

	return 0;
}

/* this function execute in dispatcher thread */
int Source_StartRoutine(Source_t *source)
{
	if( EOK!=pthread_create(NULL,NULL,Source_Routine,(void*)source))
		{return -1;}
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

/* this function execute in dispatcher thread */
static int SourceData_Ready(select_context_t *ctp, int fd, unsigned int flags, void *arg )
{
	forDataReadyFunc_t *fdrf = (forDataReadyFunc_t*)arg;
	pthread_mutex_lock(&fdrf->access->mutex);
	fdrf->access->data_ready[fdrf->ind] = 1;
	pthread_cond_signal(&fdrf->access->maywork);
	pthread_mutex_unlock(&fdrf->access->mutex);
	return 0;
}

/* this function execute in Routine thread */
static int Source_Open(SourceAccess_t *access)
{
	int i;
	for(i=0;i<SOURCE_DATA_READY_COUNT;++i)
	{
		if( -1==(access->fd[i] = open(&access->path[i][0],O_RDONLY)) )
			{return -1;}
		if( -1==(select_attach(access->dpp,NULL,access->fd[i], SELECT_FLAG_READ | SELECT_FLAG_REARM, SourceData_Ready,(void*)&forDataReadyFunc[i]) ) )
			{return -1;}
	}
	pthread_mutex_lock(&access->mutex);
	access->acces_flag = 1;
	pthread_mutex_unlock(&access->mutex);
	return 0;
}

/* this function execute in Routine thread */
static int RoundRobinCheckFlags(SourceAccess_t *access)
{
	int i;
	for(i=0;i<SOURCE_DATA_READY_COUNT;++i)
	{
		access->way = (access->way+1)%SOURCE_DATA_READY_COUNT;
		if( 1==access->data_ready[access->way] )
		{
			access->data_ready[access->way] = 0;
			return (access->way+1);/*one ready return some from 1..N, 0 - no one ready*/
		}
	}
	return 0;/*no one ready*/
}

/* this function execute in Routine thread */
static int Source_Work(Source_t *source)
{
	while(1) {
	DFLINE;
	pthread_mutex_lock(&source->access.mutex);
	while( 1==source->access.acces_flag && 0==(RoundRobinCheckFlags(&source->access)) )
	{
		pthread_cond_wait(&source->access.maywork,&source->access.mutex);
	}
	pthread_mutex_unlock(&source->access.mutex);
	if( 0==source->access.acces_flag )
		{return 0;}/*move to close*/

	/* read and process, you can't use pps thougth this way */
	switch(source->access.way+1)
	{
		case 1:
			printf("ReadAndParseFd1\n");
			break;
		case 2:
			printf("ReadAndParseFd2\n");
			break;
		default:
			break;
			/*unreacheble*/
	}

	}
	return 0;/*unreacheble*/
}

/* this function execute in Routine thread */
static int Source_Close(SourceAccess_t *access)
{
	int i;
	for(i=0;i<SOURCE_DATA_READY_COUNT;++i)
	{
		close(access->fd[i]);
		select_detach(access->dpp,access->fd[i]);
		access->fd[i]= -1;
		access->data_ready[i] = 0;
	}
	access->way = 0;

	sleep(access->connect_timeout);
	return 0;
}
