#include <stdio.h>
#include <STC15.h>
#include <stdlib.h>
#include "Delayer.h"
#include "inductor.h"

#define order_count 4

void printf_inductor_data_shake();
void printf_inductor_data_swtich();
void printf_inductor_data_between();
bit is_order=0;

unsigned char xdata S2CON_data[order_count]= {0x00};
unsigned char xdata S2CON_data_i=order_count;


void read_string_interrupt() interrupt 8 using 1
{
    unsigned long int xdata S2CON_data_exc=0;
    if (S2CON&0x01)
    {
        S2CON&=0xFE;
        if(!is_order) {
            switch (S2BUF)
            {
            case 0x00 :
                printf("ALL DATA { ");
                printf("VALUE OF NOW: %lu \n",now);
                printf_inductor_data_shake();
                printf_inductor_data_swtich();
                printf_inductor_data_between();
                printf(" }");
                break;
            case 0x01 : //Êä³öµ¥Æ¬»únow
                printf("VALUE OF NOW: %lu \n",now);
                break;
            case 0x02 :
                printf_inductor_data_shake();
                break;
            case 0x03 :
                printf_inductor_data_swtich();
                break;
            case 0x04 :
                printf_inductor_data_between();
                break;
            case 0xD0:
                table_inductor_clear_all();
                printf("Do Clear all data success \n");
                break;
            case 0xD1:
                printf("The analyze - shake:%lu \n- switch:%lu \n",table_analyze().data_shake,table_analyze().data_switch);
                break;
            case 0xFE:
                is_order=1;
                printf("Enter order mode\n");
                break;
            case 0xFF:
                is_order=0;
                printf("Execute and Close order mode\n");
                break;
            default :
                printf("NO RESULT");
            }
        }
        else {
            if (S2CON_data_i >=order_count) {
                for (S2CON_data_i=(order_count-1);S2CON_data_i>=0; S2CON_data_i--) {
                    S2CON_data_exc=S2CON_data_exc<<8;
                    S2CON_data_exc |=S2CON_data[S2CON_data_i];
                }
                S2CON_data_i=0;
            }
            S2CON_data[S2CON_data_i]=S2BUF;
            S2CON_data_i++; 
            }
    }
}


void printf_inductor_data_shake()
{
    char idata i;
    printf("The shake data :\n");
    for (i=0; i<table_array_inductor_shake_data_count; i++)
    {

        printf("%lu \n",table_inductor_data_shake[i]);
    }

}

void printf_inductor_data_swtich()
{
    char idata i;
    printf("The switch data :\n");
    for (i=0; i<table_array_inductor_data_switch_count; i++)
    {

        printf("%lu \n",table_inductor_data_switch[i]);
    }

}


void printf_inductor_data_between()
{
    char idata i;
    printf("The between data :\n");
    for (i=0; i<(table_array_inductor_data_between_count); i++)
    {
        printf("%lu \n",table_inductor_data_between[i]);
    }

}
