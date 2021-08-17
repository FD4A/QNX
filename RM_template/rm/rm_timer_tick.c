#include "../rm_template.h"

int rm_template_timer_tick(resmgr_context_t *ctp, int code, unsigned int flags, void *handle)
{
	NotifyClients(&devAttr[0]);
	return 0;
}
