#ifndef RM_TRAIN_H_
#define RM_TRAIN_H_

#include "rm/rm_attr.h"

#define DEBUG_FLINE printf("%s(%d)\n",__FUNCTION__,__LINE__)

/*************************** CONNECT *****************************************/
int io_open
(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
/**************************** IO **********************************************/
int io_devctl
(resmgr_context_t *ctp, io_devctl_t *msg, RESMGR_OCB_T *ocb);

int io_notify
(resmgr_context_t *ctp, io_notify_t *msg, RESMGR_OCB_T *ocb);

int io_close_ocb
(resmgr_context_t *ctp, void* reserved, RESMGR_OCB_T *ocb);

IOFUNC_OCB_T* new_ocb_calloc
(resmgr_context_t *ctp, IOFUNC_ATTR_T *attr);

void new_ocb_free
(IOFUNC_OCB_T* ocb);
/**************************** Timer ******************************************/
int rm_template_timer_create
(dispatch_t *dpp, uint64_t pause_nsec, uint64_t interval_nswc);

int rm_template_timer_tick
(resmgr_context_t *ctp, int code, unsigned int flags, void *handle);
/**************************** Sources*** ************************************/
#include "source/source.h"
typedef struct
{
	Source_t Source;
} DataServers_t;
/**************************** Source disc ************************************/
int rm_source_disconnect
(message_context_t *ctp, int code, unsigned flags, void *handle);


#endif /* RM_TRAIN_H_ */
