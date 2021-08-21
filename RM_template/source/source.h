#ifndef SOURCE_H_
#define SOURCE_H_

#include <pthread.h>
#include <sys/dispatch.h>

#define SOURCE_PATH_MAX_STR_LEN 150+1

#define SOURCE_DATA_READY_COUNT 2
typedef struct
{
	dispatch_t *dpp;
	unsigned connect_timeout;
	pthread_mutex_t mutex;
	pthread_rwlock_t mutex;
	pthread_cond_t maywork;
	volatile int acces_flag;
	int fd[SOURCE_DATA_READY_COUNT];
	char path[SOURCE_DATA_READY_COUNT][SOURCE_PATH_MAX_STR_LEN];
	/* if you need more fd than one, count of data ready flags also must be
	   increased because in Source_Work you must understand which concrete
	   need to be parsed*/
	volatile int data_ready[SOURCE_DATA_READY_COUNT];
	int way;/*cycle index+1 in data_array for RoundRobin fd reading*/
} SourceAccess_t;

typedef struct
{
	SourceAccess_t access;
	int data1;
	int data2;
} Source_t;

/*before StartRoutine*/
int Source_Init(Source_t *source, dispatch_t *dpp, char *path[], unsigned connect_timeout);

/*start thread that work with source*/
int Source_StartRoutine(Source_t *Source);
	void *Source_Routine(void* arg);

/*use in rm_source_disconnect*/
void Source_AccesFlagDownAndNotify(Source_t *Source);

#endif /* SOURCE_H_ */
