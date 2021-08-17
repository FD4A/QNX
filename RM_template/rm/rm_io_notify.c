#include "../rm_template.h"

int io_notify(resmgr_context_t *ctp, io_notify_t *msg, RESMGR_OCB_T *ocb)
{
	DEBUG_FLINE;
	return ( iofunc_notify(ctp,msg,ocb->old_ocb.attr->notify,0,NULL,NULL) );
}

void NotifyClients(device_attr_t *da)
{
	iofunc_notify_trigger(da->notify,1,0);
}
