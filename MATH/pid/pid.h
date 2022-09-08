#ifndef __PID_H_
#define __PID_H_
typedef struct{

    float Kp;
    float Ki;
    float Kd;
}PID;

typedef struct{

    float error;
    float lasterror;
    float differential;
    float differentialFliter;
    float pOut;
    float iOut;
    float dOut;
    float value;
}PIDOut;

float PID_Control(PID *PIDpara, PIDOut *PIDstatus, float expect_PID, float feedback_PID, float PIDtime, float Integrallimiter);
void PID_Init(void);
#define lowpass_filter  7.9577e-3


extern PID tower_yaw_pidara;
extern PIDOut tower_yaw_pidout;
extern PID tower_dis_pidara;
extern PIDOut tower_dis_pidout;
#endif
