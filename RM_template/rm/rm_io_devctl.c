#include "../rm_template.h"
#include "../rm_api.h"

int io_devctl(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb)
{
	DEBUG_FLINE;
	int sts = iofunc_devctl_default(ctp,msg,&ocb->old_ocb);
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
			case DCMD_1:
				printf("input = %d\n", *((int*)data) );
				NotifyClients(&devAttr[0]);
				break;
			default:
				MsgError(ctp->rcvid,ENOTTY);
				DEBUG_FLINE;
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

