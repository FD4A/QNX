#ifndef RM_TRAIN_H_
#define RM_TRAIN_H_

#include "rm/rm_attr.h"

#define DEBUG_FLINE printf("%s(%d)\n",__FUNCTION__,__LINE__)

/**************************** IO **********************************************/
int io_notify
(resmgr_context_t *ctp, io_notify_t *msg, RESMGR_OCB_T *ocb);
/**************************** Timer ******************************************/
int rm_template_timer_create
(dispatch_t *dpp, uint64_t pause_nsec, uint64_t interval_nswc);

int rm_template_timer_tick
(resmgr_context_t *ctp, int code, unsigned int flags, void *handle);


#endif /* RM_TRAIN_H_ */
