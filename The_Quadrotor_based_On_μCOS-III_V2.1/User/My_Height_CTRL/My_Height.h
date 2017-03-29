




extern float alt_KS103_1;           //误差值。
extern float KS103_ALT_V;			//超声波测算出的速度
extern float KS103_Alt;			//转换成M后的超声波数据

extern float tr_tmep1,thr_last,thr_tmep1,thr_tmep,  /*当前超声波油门调节量。*/
THR_Lock;

void KS103_PID_Para(void);
void KS103_CONTROL(float KS103_Alt_Target_1);
