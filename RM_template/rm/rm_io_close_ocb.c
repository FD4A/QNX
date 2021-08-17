#include "../rm_template.h"

int io_close_ocb(resmgr_context_t *ctp, void *msg, RESMGR_OCB_T *ocb)
{
	DEBUG_FLINE;
	return (iofunc_close_ocb_default(ctp,msg,&ocb->old_ocb));
}

IOFUNC_OCB_T* new_ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr)
{
	DEBUG_FLINE;
	IOFUNC_OCB_T *ocb = calloc(1,sizeof(IOFUNC_OCB_T));
	return ocb;
}

void new_ocb_free(IOFUNC_OCB_T* ocb)
{
	DEBUG_FLINE;
	free(ocb);
}
