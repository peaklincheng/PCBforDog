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
脉冲信号函数
计时器频率1HZ
*****************/


void Timer3Init(void)		//1毫秒@11.0592MHz
{
    T4T3M |= 0x02;		//定时器时钟1T模式
    T3L = 0xCD;		//设置定时初值
    T3H = 0xD4;		//设置定时初值
    IE2 |=0x20; //允许中断
    Timer3_run();

}
void Timer3_stop()
{
//重置T3
    T4T3M &= ~0x08;		//定时器3暂停
}
void Timer3_run()
{
    T4T3M |= 0x08;		//定时器3开始计时
}

void Timer3Interrupt(void) interrupt 19
{

    if(motor2_pulse>0)
    {
       io_motor2_pul=~io_motor2_pul;
    }
    else
    {
        T4T3M &= ~0x08;		//定时器3暂停
    }
		motor2_pulse--;

}

/*************************************************************MOTOR2*******E










/*****波特率发生器***********************************************S***/

void UartInit(void)		//9600bps@11.0592MHz
{
    S2CON = 0x50;		//8位数据,可变波特率
    AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
    T2L = 0xE0;		//设定定时初值
    T2H = 0xFE;		//设定定时初值
   // IE2|=0x01; //串口中断
    IP2|=0x01; //设置串口中断高优先级
    AUXR |= 0x10;		//启动定时器2
}
/*****波特率发生器****************************************************E***/



/*******************全局定时器****************************************S*/
void InitTimer0(void)
{
    AUXR |= 0x80;		//定时器时钟1T模式
    TMOD &= 0xF0;		//设置定时器模式
    TL0 = 0xCD;		//设置定时初值
    TH0 = 0xD4;		//设置定时初值
    TF0 = 0;		//清除TF0标志
    EA=1;
    ET0=1;
    TR0 = 1;		//定时器0开始计时

}


void Timer0Interrupt(void) interrupt 1 using 2
{
    now++;
    if (now >=maxtime)
    {
        now_tf++;
        now=0;
        IAP_CONTR=0x20; //重启

    }
}

/*******************全局定时器***************************************E*/






/*******************Delay***************************/

void wait(unsigned int t)
//以定时器为基础，等待多少毫秒。
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



