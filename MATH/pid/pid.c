#include "pid.h"
#include "limits.h"

PID tower_yaw_pidara;
PIDOut tower_yaw_pidout;

PID tower_dis_pidara;
PIDOut tower_dis_pidout;

void PID_Init()
{
	tower_yaw_pidara.Kp = -1.f;
	tower_yaw_pidara.Ki = 0.f;
	tower_yaw_pidara.Kp = 0.f;
	
	tower_dis_pidara.Kp = 2.5f;
	tower_dis_pidara.Ki = 0.f;
	tower_dis_pidara.Kd = 0.f;
}


// ����PID����
float PID_Control(PID *PIDpara, PIDOut *PIDstatus, float expect_PID, float feedback_PID, float PIDtime, float Integrallimiter){

    PIDstatus->error = expect_PID - feedback_PID;
    PIDstatus->differential = (PIDstatus->error - PIDstatus->lasterror)/PIDtime;

    PIDstatus->differential = PIDstatus->differentialFliter + (PIDtime / (lowpass_filter + PIDtime)) * (PIDstatus->differential - PIDstatus->differentialFliter);

    PIDstatus->differentialFliter = PIDstatus->differential;

    PIDstatus->lasterror = PIDstatus->error;

    PIDstatus->pOut = PIDpara->Kp * PIDstatus->error;
    PIDstatus->iOut += PIDpara->Ki * PIDstatus->error;
    PIDstatus->dOut = PIDpara->Kd * PIDstatus->differential;
    PIDstatus->iOut = Limits_data(PIDstatus->iOut,Integrallimiter,-Integrallimiter);

    PIDstatus->value = PIDstatus->pOut + PIDstatus->iOut + PIDstatus->dOut;

    return PIDstatus->value;
}


// ���ַ���PID   �����趨��������ֵʱ��I������Ч������PD���ƣ���֮��ʹ��PID����
float IntegralSeparation_PID_Control(PID *PIDpara, PIDOut *PIDstatus, float expect_PID, float feedback_PID , float PIDtime
                                    , float Integrallimiter,float SeparationThreshold, float SeparationConditions,float LowpassFilter){

    PIDstatus->error = expect_PID - feedback_PID;
    PIDstatus->differential = (PIDstatus->error - PIDstatus->lasterror)/PIDtime;

    PIDstatus->differential = PIDstatus->differentialFliter + (PIDtime / (LowpassFilter + PIDtime)) * (PIDstatus->differential - PIDstatus->differentialFliter);

    PIDstatus->differentialFliter = PIDstatus->differential;

    PIDstatus->lasterror = PIDstatus->error;

    PIDstatus->pOut = PIDpara->Kp * PIDstatus->error;
    if(SeparationConditions< SeparationThreshold)
        PIDstatus->iOut += PIDpara->Ki * PIDstatus->error;
    else
        PIDstatus->iOut = 0 ;
    PIDstatus->dOut = PIDpara->Kd * PIDstatus->differential;

    PIDstatus->iOut = Limits_data(PIDstatus->iOut,Integrallimiter,-Integrallimiter);

    PIDstatus->value = PIDstatus->pOut + PIDstatus->iOut + PIDstatus->dOut;

    return PIDstatus->value;
}

// ��Ƿ���PID   �����趨��ǵ���ֵʱ��ֻע�ض���ƽ����ƣ������������ϵĿ��ƽ���˥��
float DipSeparation_PID_Control(PID *PIDpara, PIDOut *PIDstatus, float expect_PID, float feedback_PID , float PIDtime
                                , float Integrallimiter,float SeparationConditions,float AttenuationCoefficient,float LowpassFilter){

    PIDstatus->error = AttenuationCoefficient *(expect_PID - feedback_PID) ;
    PIDstatus->differential = (PIDstatus->error - PIDstatus->lasterror)/PIDtime;

    PIDstatus->differential = PIDstatus->differentialFliter + (PIDtime / (LowpassFilter + PIDtime)) * (PIDstatus->differential - PIDstatus->differentialFliter);

    PIDstatus->differentialFliter = PIDstatus->differential;

    PIDstatus->lasterror = PIDstatus->error;

    PIDstatus->pOut = PIDpara->Kp * PIDstatus->error;

    PIDstatus->iOut += PIDpara->Ki * PIDstatus->error;

    PIDstatus->dOut = PIDpara->Kd * PIDstatus->differential;

    PIDstatus->iOut = Limits_data(PIDstatus->iOut,Integrallimiter,-Integrallimiter);

    PIDstatus->value = PIDstatus->pOut + PIDstatus->iOut + PIDstatus->dOut;

    return PIDstatus->value;
}

