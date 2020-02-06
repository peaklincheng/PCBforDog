#include "STC15.h"
#include "Delayer.h"
#include "IO_PORT.h"
#include "motor.h"
#include "WatchDog.h"
#include "inductor.h"
#include "Water.h"
#include "threading.h"


unsigned long int idata timerTime=7200000; //定时启ms
//感应器触发时间间隔，在这里个时间内只能触发一次
unsigned long int idata interval_time=900000;

unsigned long int idata interval_last_time=0;

void timer_run(void);
void timeLine(void);
void en_bt(void);
void runLine(void);
char runLine_state=-1;//流水线的状态，0开始执行，1执行完成
void inductor_run();
unsigned long int idata now_start;
unsigned long int idata led_timer_start;


void timeLine(void)
{

    now_start=now;//初始化定时函数变量
    led_timer_start=now;
    while(1)
    {
        watch_feed_dog();//喂狗
        en_bt();
        timer_run();
        inductor_run();
        if(now-led_timer_start >1000)
        {
            led_timer_start=now;
            P55=~P55;

        }

    }


}






void runLine(void)
{
    
    inductor_close_all();
    motor2_make_zero();
    water_op();
    wait(5000);
    water_op();
		motor2_move(-100,10);
	  motor2_move(-100,10);
	  motor_run();
	  motor2_move(-100,10);
  	motor2_move(-100,10);
		motor2_move(-25,4);
    wait(5000);
    water_cl();
	  wait(5000);
		motor_stop();
    motor2_make_zero();
    inductor_open_all();
    interval_last_time=now;
}



void timer_run(void)
{
//定时启动函数
    if(now>= (now_start+timerTime ))
    {
        inductor_close_all();//关闭所有感应器
        runLine();
        now_start=now;
        inductor_open_all();//开启所有感应器
    }
}




void inductor_run()
{
		unsigned long int idata temp=(now-inductor_en_time_start);
		unsigned long int idata temp2=(now-interval_last_time);
    if(inductor_en==1 && io_inductor==1 && temp>120000 && temp2 > interval_time)
    {
        inductor_close_all();//关闭所有感应器
        if((table_analyze()).data_shake)
        {
            runLine();
            indector_last_time=now;
            inductor_en=0;
					  table_inductor_clear_all();
        }
        else
        {
            inductor_en=0;
        }
        inductor_open_all();//开启所有感应器
    }
}











/********
en_bt 函数
检查按键，并作出相应功能
此函数不涉及中断
*********/

void en_bt(void)
{
    if(io_bt_motor2_back==0)
    {
			delay500ms();
			  IE2&=~0x01; //串口中断
        motor2_make_zero();
    }

    if(io_bt_motor2_go==0)
    {
			delay500ms();
				 IE2|=0x01; //串口中断
         io_motor_ena=~io_motor_ena;
    }
    if(io_bt_water_ena==0)
    {

        delay500ms();
        water_set();
    }
    if(io_bt_inductor_en==0)
    {

        runLine();

    }

}




