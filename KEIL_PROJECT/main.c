#include "STC15.h"
#include "SignalCpu.h"
#include "Delayer.h"
#include "Water.h"
#include "IO_PORT.h"
#include "motor.h"
#include "WatchDog.h"
#include "inductor.h"
#include "threading.h"

void main(void);
void __init__(void);
void motor(void);
void set_io_mode(void);
void main(void)
{
    __init__();
    timeLine();

}



/**********************************************
FuncName:__init__
info:��ʼ������

***********************************************/
void __init__(void)
{
    set_io_mode();
		IAP_CONTR |= 0x40;
    delay2s();
    InitTimer0(); //����ȫ��ʱ��(�룩
    motor_init();//�����ʼ����������ʱ��
    watch_dog_init();//��ʼ�����Ź�
    inductor_init();
    UartInit();//����2
    motor2_stop();
		motor_stop();
}

void set_io_mode(void){
    P0M0=0x00;
    P1M0=0x00;
    P2M0=0x00;
    P3M0=0x00;
    P4M0=0x00;
    P0M1=0x00;
    P1M1=0x00;
    P2M1=0x00;
    P3M1=0x00;
    P4M1=0x00;
    P4M0|=0x00;
	  //����ǿ��ģʽ-----S
		P0M1&=0xF0;
		P0M0|=0x0F;
		P5M1&=0xDF;
		P5M0|=0x20;
	 //����ǿ��ģʽ------E
    P0=0xFF;
		P1=0XFF;
    P2=0xFF;
    P3=0xFF;
    P4=0xFF;
		P0&=0xF0;
		P5&=0xDF;
	
		

}


