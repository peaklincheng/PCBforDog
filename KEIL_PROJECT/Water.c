#include "STC15.h"
#include "IO_PORT.h"
#include "delayer.h"

void water_op(void);
void water_cl(void);
void water_set(void);

void water_op(void)
{
    INT_CLKO &=~(0x10); //INT3 �ж�����λ
    io_water=1;
    delay500ms();
    INT_CLKO |=0x10; //INT3 �ж�����λ

}

void water_cl(void)
{
    INT_CLKO &=~(0x10); //INT3 �ж�����λ
    io_water=0;
    delay500ms();
    INT_CLKO |=0x10; //INT3 �ж�����λ
}
void water_set(void)
{
    INT_CLKO &=~(0x10); //INT3 �ж�����λ
    io_water=~io_water;
    delay500ms();
    INT_CLKO |=0x10; //INT3 �ж�����λ
}

