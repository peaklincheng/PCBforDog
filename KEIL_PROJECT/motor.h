extern void motor_init(void);
extern void motor_set_dir(void);
extern void motor_set_ena(void);
extern void set_speed_add(void);
extern void set_speed_sub(void);
extern void motor_run();
extern void motor_stop();
extern void motor2_run();
extern void motor2_stop();
extern void motor2_make_zero();
extern void motor2_move(int distance,unsigned char t);