#include "../rm_template.h"

int rm_source_disconnect(message_context_t *ctp, int code, unsigned flags, void *handle)
 {
	DataServers_t *ds = (DataServers_t*)handle;
	int fd = ctp->msg->pulse.value.sival_int;
	if( fd==ds->Source.access.fd)/*need we lock here?*/
		{Source_AccesFlagDownAndNotify(&ds->Source);}
	return 0;
 }

