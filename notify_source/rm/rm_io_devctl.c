#include "../notify_source.h"
#include <devctl.h>

int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
	int sts = iofunc_devctl_default(ctp,msg,ocb);
	if(_RESMGR_DEFAULT!=sts)
		{return sts;}
	int nbytes=0;
	void *data;
	data = _DEVCTL_DATA(msg->i);

	/* you may register and redefine io_func and connect_func separately
	 * for each device and write io_devctl for each of them without if*/
	if(devAttr[0].id == ctp->id)
	{
		switch(msg->i.dcmd)
		{
			default:
				MsgError(ctp->rcvid,ENOTTY);
				break;
		}
	}
	else
	{
		MsgError(ctp->rcvid,ENOENT);
		return (_RESMGR_NOREPLY);
	}

	return ( _RESMGR_PTR(ctp,&msg->o,sizeof(msg->o)+nbytes) );
}

