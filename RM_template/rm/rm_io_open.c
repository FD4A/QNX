#include "../rm_template.h"

int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra)
{
	DEBUG_FLINE;
	return ( iofunc_open_default(ctp, msg, &handle->attr, extra) );
}
