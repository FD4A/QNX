#ifndef SOURCE_H_
#define SOURCE_H_

#include <pthread.h>

#define SOURCE_PATH_MAX_STR_LEN 150+1

typedef struct
{
	char path[SOURCE_PATH_MAX_STR_LEN];
	int fd;
	unsigned connect_timeout;
	pthread_mutex_t mutex;
	pthread_cond_t maywork;
	volatile int acces_flag;
	volatile int data_ready;/*if you need more fd than one, count of data ready flags
	also must be increased because in Source_Work you must understand which concrete
	need to be parsed*/
} SourceAccess_t;

typedef struct
{
	SourceAccess_t access;
	int data;
} Source_t;

int Source_Init(Source_t *source, char *path, unsigned connect_timeout);/*before StartRoutine*/
int Source_StartRoutine(Source_t *Source);/*start thread that work with source*/
void *Source_Routine(void* arg);
void Source_AccesFlagDownAndNotify(Source_t *Source);/*use in rm_source_disconnect*/

#endif /* SOURCE_H_ */
