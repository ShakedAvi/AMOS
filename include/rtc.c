#include "rtc.h"

time_t global_time;
int32 bcd;

void gettime(time_t *time)
{
    memory_copy(&global_time, time, sizeof(time_t));
}

void rtc_handler(struct regs* r)
{
    if(read_register(0x0C) & 0x10)
    {
        if(bcd)
        {
            global_time.second = bcd2bin(read_register(0x00));
            global_time.minute = bcd2bin(read_register(0x02));
            global_time.hour   = bcd2bin(read_register(0x04));
            global_time.month  = bcd2bin(read_register(0x08));
            global_time.year   = bcd2bin(read_register(0x09));
            global_time.day_of_week  = bcd2bin(read_register(0x06));
            global_time.day_of_month = bcd2bin(read_register(0x07));
        }
        else
        {
            global_time.second = read_register(0x00);
            global_time.minute = read_register(0x02);
            global_time.hour   = read_register(0x04);
            global_time.month  = read_register(0x08);
            global_time.year   = read_register(0x09);
            global_time.day_of_week  = read_register(0x06);
            global_time.day_of_month = read_register(0x07);
        }
    }
}

void rtc_install(void)
{
    uint8 status;

    status = read_register(0x0B);
    status |=  0x02;             // 24 hour clock
    status |=  0x10;             // update ended interrupts
    status &= ~0x20;             // no alarm interrupts
    status &= ~0x40;             // no periodic interrupt
    bcd     =  !(status & 0x04); // check if data type is BCD
    write_register(0x0B, status);

    read_register(0x0C);

    register_interrupt_handler(40, (void*)rtc_handler);
}
