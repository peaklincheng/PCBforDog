#include <math.h>
#include <STC15.h>

#define thread_count 10 //列队大小
#define maxtime 2147483647

unsigned long int now=0;
unsigned long int now_tf=0;//时间溢出次数
unsigned char thread_empty();
void threadCreate_init();
void _threading_init();
struct threadObjects
{
    unsigned char location;//当前位置
    void (*func)(struct threadObjects *cthis);//当前函数
    unsigned long int extime;//执行时间（等待时间+加入时间）
    unsigned char tf;//时间重置次数
    void (*nextFunc)(struct threadObjects *cthis);//下一个函数指针 *cthis 是传入当前实例地址
};


struct threads
{
    unsigned char cize;
    struct threadObjects *p[thread_count+1];//列表
    void (*f)(void);//循环执行线程
    void (*add)(void (*p)(struct threadObjects *cthis),unsigned char location);//加入线程函数

} idata thread;


struct threadObjects xdata threadObjectList[thread_count];





//循环执行线程
void _threads_line(void)
{
    int i;
    for(i=1; i<(thread.cize); i++)
    {
        if(thread.p[i])
        {
            if((( (now_tf-thread.p[i]->tf)*maxtime +now)-thread.p[i]->extime )>=0 )

            {
                thread.p[i]->func(thread.p[i]);
                if (thread.p[i]->nextFunc)
                {
                    //增加到列表，替换当前列表
                    thread.add((thread.p[i]->nextFunc),(thread.p[i]->location));

                }
            }

        }
    }
}



void _thread_add(void (*p)(struct threadObjects *cthis),unsigned char location)
{
    unsigned char em;
    struct threadObjects obj;
    obj.tf=now_tf;
    obj.func=p;
    if (location)
    {
        obj.location=location;
    }
    else
    {
        em=thread_empty();
        if (em)
        {
            obj.location=thread_empty();
        }

    }

    if(location)
    {
        *(thread.p[obj.location])=obj;
    }

}

void threadCreate(void (*p)(void)){

    struct threadObjects obj;
    obj.tf=now_tf;
    obj.extime=0;
    obj.func=threadCreate_init;
    obj.nextFunc=p;
    obj.location=thread_empty();
    if (obj.location){
        *(thread.p[obj.location])=obj;
    }

}

void threadCreate_init(void *(p)(struct threadObjects (*cthis)))
    //create的构造函数
{
    //什么都不做

}





unsigned char thread_empty()
{
//返回一个最近的空指针数组下标
    int i;
    for(i=1; i<thread.cize; i++)
    {
        if (!thread.p[i])
        {
            return i;
            break;
        }

    }
    return 0;

}

void _threading_init()
{
    char i;
    thread.add=_thread_add;
    thread.f=_threads_line;
    thread.cize=thread_count+1;
    for(i=1;i<thread.cize;i++)
	 {
        thread.p[i]=&threadObjectList[i-1]; //初始化指针赋值
    }
}


































