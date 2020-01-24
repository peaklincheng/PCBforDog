#include "STC15.h"


void watch_dog_init(void);
void watch_feed_dog(void);
void watch_dog_init(void)
{
    WDT_CONTR=0x37;
}

void watch_feed_dog(void)
{
    WDT_CONTR=0x37;
}
