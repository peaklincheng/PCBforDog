
#include "STC15.h"
#define rod_length 450


//ˮ��IO
sbit io_water=P0^0;

//���1
sbit io_motor_ena=P0^1;

//���IO2
sbit io_motor2_pul=P0^6;
sbit io_motor2_dir=P0^5;
sbit io_motor2_ena=P0^4;





//����io
extern char io_motor_ena_v ;


//������Ӧ�ж�
sbit io_inductor=P3^2;
//̤����λ����
sbit io_menu=P3^3;


//������λ����
sbit io_motor2_zero=P3^6;



//���ܰ���
sbit io_bt_water_ena=P2^0;//ˮ����
sbit io_bt_motor2_go=P2^1;//˿����ǰ
sbit io_bt_motor2_back=P2^2; //˿�����
sbit io_bt_inductor_en=P2^3;//��Ӧ����������ر�



//����ͨѶ
sbit io_rxd=P1^0;
sbit io_txd=P1^1;
