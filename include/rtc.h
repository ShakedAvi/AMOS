#ifndef _RTC_H
#define _RTC_H

#include "system.h"
#include "isr.h"

typedef struct {
    uint8 second;
    uint8 minute;
    uint8 hour;
    uint8 day_of_week;
    uint8 day_of_month;
    uint8 month;
    uint8 year;
} time_t;


void gettime(time_t* time);
void rtc_install(void);

#endif
