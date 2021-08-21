#include "../notify_source.h"

int rm_template_timer_create(dispatch_t *dpp, uint64_t pause_nsec, uint64_t interval_nswc)
{
	int timer_id;
	struct _itimer itime;
	struct sigevent event;
	if( (event.sigev_code = pulse_attach(dpp, NULL, 0, (void*)(&rm_template_timer_tick), NULL) )== -1 )
	{
		fprintf(startlog,"fail pulse_attach() cannot start timer\n");
		return -1;
	}
	fprintf(startlog,"success pulse_attach()...\n");

	if((event.sigev_coid = message_connect(dpp, MSG_FLAG_SIDE_CHANNEL)) == -1)
	{
		fprintf(startlog,"fail message_connect()\n");
		return -1;
	}
	fprintf(startlog,"success message_connect()...\n");
	event.sigev_notify = SIGEV_PULSE;
	event.sigev_priority = -1; /*parent priority*/
	event.sigev_value.sival_int = 0;
	if((timer_id = TimerCreate(CLOCK_REALTIME, &event)) == -1)
	{
		fprintf(startlog,"fail TimerCreate()\n");
		return -1;
	}
	fprintf(startlog,"success TimerCreate()...\n");
	itime.nsec = 900000000;
	itime.interval_nsec = 800*1000*1000;
	TimerSettime(timer_id, 0, &itime, NULL);

	return 0;
}
