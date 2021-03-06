#include "STC15.h"
#include "IO_PORT.h"
#include "Delayer.h"
#include <intrins.h>
#include "WatchDog.h"
#include "SignalCpu.h"
#include "motor.h"
#include "threading.h"
#include <math.h>
#include <inductor.h>
#include <string.h>
//#include <stdio.h>
//感应数据采样数量


char idata inductor_en=0;
void inductor_init(void);
void inductor_do(void);

void table_orderBy_asc( unsigned long int *table);
void table_inductor_shake_data_write(unsigned long int da);
void table_inductor_switch_data_write(unsigned long int da);

unsigned long int idata indector_last_time=0;
unsigned long int idata inductor_en_time_start=0; //感应器触发时间

//数组加5是为了防止跑飞
unsigned long int xdata table_inductor_data_switch[table_array_inductor_data_switch_count+1];//储存限位感应触发时间
unsigned long int xdata table_inductor_data_shake[table_array_inductor_shake_data_count+1];//储存震动感应触发时间
unsigned long int xdata table_inductor_data_between[table_array_inductor_data_between_count];//储存感应值中间差
int xdata table_inductor_shake_data_i=0;
int xdata table_inductor_switch_data_i=0;
unsigned long int xdata inductor_last_shake_time=0;



void inductor_init(void)
{
    //初始化函数，中断启动..
    table_inductor_clear_all();
    EA=1;
    IT0=1;
    IT1=0;
    inductor_open_all();
}




/*********************************************************感应器数据********S*/
void table_inductor_clear_all()
{
    //感应数据数组清零
    int i=0;
    table_inductor_shake_data_i=0;
    table_inductor_switch_data_i=0;
    memset(table_inductor_data_shake,'\0',sizeof(table_inductor_data_shake));
    memset(table_inductor_data_switch,'\0',sizeof(table_inductor_data_switch));
    memset(table_inductor_data_between,'\0',sizeof(table_inductor_data_between));

}



void table_inductor_shake_data_write(unsigned long int da)
//写入数组
{
    unsigned long int xdata *p;
    if (table_inductor_shake_data_i == (table_array_inductor_shake_data_count))
    {
        table_inductor_shake_data_i=0;
    }
    p=&table_inductor_data_shake[table_inductor_shake_data_i];
    *p=da;
    table_inductor_shake_data_i++;
}




void table_inductor_switch_data_write(unsigned long int da)
//写入数组
{
    unsigned long int xdata *p;
    if (table_inductor_switch_data_i ==(table_array_inductor_data_switch_count))
    {
        table_inductor_switch_data_i=0;
    }
    p=&table_inductor_data_switch[table_inductor_switch_data_i];
    *p=da;
    table_inductor_switch_data_i++;
}




void table_orderBy_asc( unsigned long int *table)
//数组从小到大排列
{
    int i,ii;
    unsigned long int idata temp;
    unsigned long int *p1,*p2;
    for (i=0; i<(table_array_inductor_shake_data_count-1); i++)
    {
        for (ii=1; ii<table_array_inductor_shake_data_count; ii++)
        {
            if (table[ii-1]>table[ii])
            {
                p1=&table[ii-1];
                p2=&table[ii];
                temp= *p1;
                *p1=*p2;
                *p2=temp;
//                memcpy(&temp,&table[ii-1],sizeof(temp));
//                memcpy(&table[ii-1],&table[ii],sizeof(temp));
//                memcpy(&table[ii],&temp,sizeof(temp));
            }
        }
    }
}

/*********************************************************感应器数据********E*/




/**********************************************************感应器分析**********S*/
void table_math_between()
{
    //计算时间间隔值
    int i;
    unsigned long int *p;
    table_orderBy_asc(table_inductor_data_shake);
    for (i=0; i<(table_array_inductor_shake_data_count-1); i++)
    {
        if (table_inductor_data_shake[i+1] && table_inductor_data_shake[i]>0)
        {
            p=&table_inductor_data_between[i];
            *p=table_inductor_data_shake[i+1]-table_inductor_data_shake[i];

        }
        else
        {
            table_inductor_data_between[i]=0;
        }
    }
}

struct InductorAnalyzeResult table_analyze()
{
    //计算大于3s毫秒的震动间隔数据。
    int idata i,ii;
    struct InductorAnalyzeResult intersect= {0,0};
    unsigned long int idata ss=0;
    table_math_between();
    for (i=0; i<(table_array_inductor_shake_data_count); i++)
    {
        if (table_inductor_data_between[i]> 1000 ) //忽略少于3S的间隔
        {
            ss+=table_inductor_data_between[i];
        }
    }
    for (i=0; i<table_array_inductor_data_switch_count; i++)

    {
        for (ii=0; ii<table_array_inductor_shake_data_count; ii++)
        {
            if(abs(table_inductor_data_switch[i]-table_inductor_data_shake[ii])<1000 & table_inductor_data_switch[i] && table_inductor_data_shake[ii] )
                //判断震动感应和限位感应触发时间是否有相交，误差500MS
            {
                intersect.data_switch=table_inductor_data_switch[i];
                intersect.data_shake=table_inductor_data_shake[ii];
                i=table_array_inductor_data_switch_count;//退出循环
                break;
            }

        }
    }
    if (ss>10000 && intersect.data_switch)
        //如果震动感应数据间有大于10秒的，且有相交
    {
        return intersect;
    }
    else
    {
        intersect.data_switch=0;
        intersect.data_shake=0;
        return intersect;
    }


}











/**********************************************************感应器分析**********E*/



















/*****************************************************感应器中断************S*/
//震动开关中断函数 NEW！！
void inductor_shake_interrupe(void) interrupt 0
{

    if(now-inductor_last_shake_time <1000)
    {

        return;
    }
    EX0=0;
    inductor_last_shake_time=now;
    table_inductor_shake_data_write(now);
    EX0=1;
}

//限位开关感应器
void inductor_ray_interrupt(void) interrupt 2
{
    EX1=0;
    inductor_en=1;
    if (now-inductor_en_time_start >500)
    {
        table_inductor_switch_data_write(now);
        inductor_en_time_start=now;
    }
    EX1=1;
}

/*****************************************************感应器中断************E*/





/******************************************************感应器开关***********S***/

void inductor_close_all()
{
    EX0=0;
    EX1=0;
}


void inductor_open_all()
{
    EX0=1;
    EX1=1;
}

/******************************************************感应器开关**********E****/














