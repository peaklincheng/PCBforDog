
extern void InitTimer4(void (*callback)(void));
extern void delay1ms();
extern void delay2s();
extern void delay3s();
extern void delay500ms();
extern void UartInit(void);
extern void Timer3_stop();
extern void Timer3_run();
extern unsigned long int idata motor2_pulse;
extern char idata motor2_runFinish;
extern unsigned int xdata now_tf;
extern unsigned long int now;
extern void InitTimer0(void);
extern void Timer3Init(void);
extern void wait(unsigned int t);
