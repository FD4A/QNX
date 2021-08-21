#include "notify_source.h"

int main(int argc, char *argv[])
{
	/* preparing, remove or replace that default settings as you need */
	char *RM_DEV_PATH = argv[1];
	startlog = stdout;
	uint64_t pause_nsec = 900*1000*1000;
	uint64_t interval_nsec = 20*1000*1000;

    /*create dispatcher*/
	dispatch_t *dpp;
	dpp = dispatch_create();
	if(NULL==dpp)
	{
		fprintf(startlog,"fail dispatch_create(), error = %s\n",strerror(errno));
		return EXIT_FAILURE;
	}
	fprintf(startlog,"success dispatch_create()...\n");

    /*init resmgr_attr*/
	resmgr_attr_t resmgr_attr;
		memset(&resmgr_attr,0,sizeof(resmgr_attr_t));
		resmgr_attr.msg_max_size = 1024;
		resmgr_attr.nparts_max = 1;

    /*init tables of connection and input-output functions*/
	resmgr_io_funcs_t io_func;
	resmgr_connect_funcs_t connect_func;
	iofunc_func_init(_RESMGR_CONNECT_NFUNCS,&connect_func,_RESMGR_IO_NFUNCS,&io_func);
		io_func.notify = io_notify;		

	int i;
	for(i=0;i<RM_MAX_DEV_COUNT;++i)
	{
		iofunc_attr_init(&devAttr[i].attr, _S_IFNAM | 0664, NULL,NULL);
			IOFUNC_NOTIFY_INIT(devAttr[i].notify);

		/*attach device in file system*/
		devAttr[i].id = resmgr_attach(dpp,&resmgr_attr,RM_DEV_PATH,_FTYPE_ANY,0,&connect_func,&io_func,&devAttr[i]);
		if(-1==devAttr[i].id)
		{
			fprintf(startlog,"fail resmgr_attach(%s), error = %s\n",RM_DEV_PATH,strerror(errno));
			return EXIT_FAILURE;
		}
		fprintf(startlog,"success resmgr_attach(%s)...\n",RM_DEV_PATH);
	}
	
	/*create timer*/
	if( -1==rm_template_timer_create(dpp,pause_nsec,interval_nsec) )
	{
		fprintf(startlog,"fail rm_template_timer_create\n");
		return EXIT_FAILURE;
	}
	fprintf(startlog,"success rm_template_timer_create()...\n");

    /*dispatch alloc*/
    dispatch_context_t *ctp = dispatch_context_alloc(dpp);
    if( NULL==ctp )
	{
		fprintf(startlog,"fail dispatch_context_alloc(), error = %s\n",strerror(errno));
		return EXIT_FAILURE;
	}
	fprintf(startlog,"success dispatch_context_alloc()...\n");
    
	/*infinite dispatcher loop*/
    while(1)
	{
		ctp = dispatch_block(ctp);
		if(NULL==ctp)
		{
			return EXIT_FAILURE;
		}
		dispatch_handler(ctp);
	}

	return EXIT_SUCCESS;
}
