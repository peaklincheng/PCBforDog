
#include "STC15.h"
#define rod_length 450


//水阀IO
sbit io_water=P0^0;

//电机1
sbit io_motor_ena=P0^1;

//电机IO2
sbit io_motor2_pul=P0^6;
sbit io_motor2_dir=P0^5;
sbit io_motor2_ena=P0^4;





//虚拟io
extern char io_motor_ena_v ;


//振动器感应中断
sbit io_inductor=P3^2;
//踏板限位开关
sbit io_menu=P3^3;


//归零限位开关
sbit io_motor2_zero=P3^6;



//功能按键
sbit io_bt_water_ena=P2^0;//水开关
sbit io_bt_motor2_go=P2^1;//丝杆向前
sbit io_bt_motor2_back=P2^2; //丝杆向后
sbit io_bt_inductor_en=P2^3;//感应功能启动或关闭



//串口通讯
sbit io_rxd=P1^0;
sbit io_txd=P1^1;
