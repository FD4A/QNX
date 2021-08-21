#include "../rm_template.h"

int rm_source_disconnect(message_context_t *ctp, int code, unsigned flags, void *handle)
 {
	DataServers_t *ds = (DataServers_t*)handle;
	int fd = ctp->msg->pulse.value.sival_int;
	int i;
	for(i=0;i<SOURCE_DATA_READY_COUNT;++i)
	{
		if( fd==ds->Source.access.fd[i] )
			{Source_AccesFlagDownAndNotify(&ds->Source);}
	}
	return 0;
 }

