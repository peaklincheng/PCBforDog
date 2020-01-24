#define table_array_inductor_shake_data_count 60
#define table_array_inductor_data_switch_count 10
#define table_array_inductor_data_between_count table_array_inductor_shake_data_count-1+1


struct InductorAnalyzeResult {
    unsigned long int data_shake;
    unsigned long int data_switch;
};

extern void inductor_init(void);
extern char idata inductor_en;
extern unsigned long int idata inductor_en_time_start;
extern unsigned long int idata indector_last_time;
extern void inductor_open_all();
extern void inductor_close_all();
extern struct InductorAnalyzeResult table_analyze();
extern void table_inductor_clear_all();

extern unsigned long int xdata table_inductor_data_switch[table_array_inductor_data_switch_count+1];//储存限位感应触发时间
extern unsigned long int xdata table_inductor_data_shake[table_array_inductor_shake_data_count+1];//储存震动感应触发时间
extern unsigned long int xdata table_inductor_data_between[table_array_inductor_data_between_count];//储存感应值中间差
