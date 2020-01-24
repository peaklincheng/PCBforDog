#include "STC15.h"
#include "IO_PORT.h"
#include "Delayer.h"
#include "motor.h"
#include <math.h>
#include "WatchDog.h"
#include "inductor.h"
//电机方向左正右负


void set_speed(void);
unsigned int _fDistanceToPulse(unsigned char t,int distance);
void motor2_run_for_time();
void _init_motor_interrupt_swich();
char xdata zero_i;

void motor_init(void)
{

    _init_motor_interrupt_swich();
    Timer3Init();
}



void motor2_set_dir(void)
{
    io_motor2_dir=~io_motor2_dir;

}


void motor2_set_ena(void)
{
    io_motor2_ena=~io_motor2_ena;

}

void motor_run()
{
    io_motor_ena=1;

}
void motor_stop()
{
    io_motor_ena=0;

}

void motor2_run()
{
    io_motor2_ena=1;
    Timer3_run();

}
void motor2_stop()
{
    Timer3_stop();
    io_motor2_ena=0;

}



/***在t秒时间内移动丝杆distance 毫米****/
void motor2_move(int distance,unsigned char t)
{
    //distance 不能超过150，t和distance 比值 最大 1：1.5
    motor2_stop();
    _init_motor_interrupt_swich();
    inductor_close_all();
    motor2_runFinish=0;
    if (_fDistanceToPulse(t,distance)>0)
    {
        motor2_run_for_time();
    };
    inductor_open_all();
}









void motor2_run_for_time()
{
    motor2_run();
    while (1)
    {
        watch_feed_dog();//喂狗
        if (motor2_pulse<=0)
        {
            motor2_stop();
            break;
        }
    }
}



void motor2_make_zero()
{
    //步进电机归零
    _init_motor_interrupt_swich();
    motor2_stop();
    for (zero_i=0; zero_i<(rod_length+1)/10; zero_i++)
    {
        motor2_move(10,1);
    }
}


void _init_motor_interrupt_swich()
{
    //电机限位开关初始化中断
    INT_CLKO |=0x10; //INT3 中断允许位
}





void motor2_Switch_interrupt() interrupt 10
{
    motor2_stop();
    _fDistanceToPulse(1,-5);
    motor2_run();
    zero_i=125;
}




/**计算脉冲,输入完成所需时间和移动距离mm（正数为向前，负数向后）
直接设置脉冲数量，和返回脉冲值。*/

unsigned int _fDistanceToPulse(unsigned char t,int distance)
{
    unsigned long int pulse;
    unsigned int idata tempc;
    unsigned long int idata* pp;
    unsigned long int idata* pmp;
    if (distance >0)
    {
        io_motor2_dir=1;
    }
    else
    {
        io_motor2_dir=0;
    }
    pmp=&pulse;
    pp=&motor2_pulse;
    Timer3_stop();
    *pmp=(unsigned int)(abs(distance)*50*4*2); //*32是驱动器分频 *2是脉冲只认上沿
    tempc=65536-(int)(11059200.0*((float)t / *pmp));
    if(tempc<=0)
    {
        return 0;
    }
    else
    {
        T3L = (tempc<<8)>>8;		//设置定时初值
        T3H = tempc>>8;		//设置定时初值
        *pp=pulse;
        return (int)tempc;
    }

}






















