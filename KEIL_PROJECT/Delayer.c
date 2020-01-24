#include "STC15.h"
#include "Delayer.h"
#include <intrins.h>
#include "IO_PORT.h"
#include "WatchDog.h"
unsigned long int idata maxtime=2147483647;

void Timer0Interrupt(void) ;

unsigned long int idata motor2_pulse=0;
char idata motor2_runFinish=1;

unsigned int xdata now_tf;
unsigned long int now;






/*************************************************************MOTOR2*******S
�����źź���
��ʱ��Ƶ��1HZ
*****************/


void Timer3Init(void)		//1����@11.0592MHz
{
    T4T3M |= 0x02;		//��ʱ��ʱ��1Tģʽ
    T3L = 0xCD;		//���ö�ʱ��ֵ
    T3H = 0xD4;		//���ö�ʱ��ֵ
    IE2 |=0x20; //�����ж�
    Timer3_run();

}
void Timer3_stop()
{
//����T3
    T4T3M &= ~0x08;		//��ʱ��3��ͣ
}
void Timer3_run()
{
    T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
}

void Timer3Interrupt(void) interrupt 19
{

    if(motor2_pulse>0)
    {
       io_motor2_pul=~io_motor2_pul;
    }
    else
    {
        T4T3M &= ~0x08;		//��ʱ��3��ͣ
    }
		motor2_pulse--;

}

/*************************************************************MOTOR2*******E










/*****�����ʷ�����***********************************************S***/

void UartInit(void)		//9600bps@11.0592MHz
{
    S2CON = 0x50;		//8λ����,�ɱ䲨����
    AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
    T2L = 0xE0;		//�趨��ʱ��ֵ
    T2H = 0xFE;		//�趨��ʱ��ֵ
   // IE2|=0x01; //�����ж�
    IP2|=0x01; //���ô����жϸ����ȼ�
    AUXR |= 0x10;		//������ʱ��2
}
/*****�����ʷ�����****************************************************E***/



/*******************ȫ�ֶ�ʱ��****************************************S*/
void InitTimer0(void)
{
    AUXR |= 0x80;		//��ʱ��ʱ��1Tģʽ
    TMOD &= 0xF0;		//���ö�ʱ��ģʽ
    TL0 = 0xCD;		//���ö�ʱ��ֵ
    TH0 = 0xD4;		//���ö�ʱ��ֵ
    TF0 = 0;		//���TF0��־
    EA=1;
    ET0=1;
    TR0 = 1;		//��ʱ��0��ʼ��ʱ

}


void Timer0Interrupt(void) interrupt 1 using 2
{
    now++;
    if (now >=maxtime)
    {
        now_tf++;
        now=0;
        IAP_CONTR=0x20; //����

    }
}

/*******************ȫ�ֶ�ʱ��***************************************E*/






/*******************Delay***************************/

void wait(unsigned int t)
//�Զ�ʱ��Ϊ�������ȴ����ٺ��롣
{
    unsigned long int idata start_time;
    start_time=now;
    while (1)
    {
        watch_feed_dog();
        if ((now-start_time)>t)
        {
            break;
        }
    }

}












void delay2s()		//@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 85;
    j = 12;
    k = 155;
    do
    {
        do
        {
            while (--k);
        }
        while (--j);
    }
    while (--i);
}


void delay1ms()		//@11.0592MHz
{
    unsigned char i, j;

    _nop_();
    _nop_();
    _nop_();
    i = 11;
    j = 190;
    do
    {
        while (--j);
    }
    while (--i);
}



void delay500ms()		//@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 22;
    j = 3;
    k = 227;
    do
    {
        do
        {
            while (--k);
        }
        while (--j);
    }
    while (--i);
}



void delay3s()		//@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 127;
    j = 18;
    k = 107;
    do
    {
        do
        {
            while (--k);
        }
        while (--j);
    }
    while (--i);
}



