#ifndef RM_ATTR_H_
#define RM_ATTR_H_

/*redefine device attr*/
struct device_attr_s;
#define IOFUNC_ATTR_T struct device_attr_s

/*redefine ocb*/
struct new_ocb_s;
#define IOFUNC_OCB_T struct new_ocb_s

#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <sys/neutrino.h>
#include <sys/resmgr.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <time.h>
#include <string.h>

typedef struct device_attr_s
{
	iofunc_attr_t attr;
	iofunc_notify_t notify[3];
	int nitems;
	int id;
}device_attr_t;

#define RM_MAX_DEV_COUNT 1
device_attr_t devAttr[RM_MAX_DEV_COUNT];

typedef struct new_ocb_s
{
	iofunc_ocb_t old_ocb;
} new_ocb_t;

FILE *startlog;

void NotifyClients(device_attr_t *da);

#endif /* RM_ATTR_H_ */
