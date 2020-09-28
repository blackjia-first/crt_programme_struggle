#include "PID.h"
 
PID_t speed_pid_1,pos_pid_1,speed_pid_2,pos_pid_2;

//构造函数d
 void pid_init(PID_t * p_pid, float SumErrorMax, float Kp, float Ki, float Kd, float OutputMax)
{
  p_pid->m_SumErrorMax = SumErrorMax;
	p_pid->m_Kp = Kp;
	p_pid->m_Ki = Ki;
	p_pid->m_Kd = Kd;
	p_pid->m_OutputMax = OutputMax;
	
}

//PID对象获取反馈值
void SetFeedBack(PID_t * p_pid,float FeedBack)
{
   p_pid->m_FeedBack = FeedBack;
}

//PID对象设置目标值
void SetDes(PID_t * p_pid,float Des)
{
	p_pid->m_Des= Des;
}
//PID计算
void Calculate(PID_t * p_pid)
{
	p_pid->m_Errorm = p_pid->m_Des - p_pid->m_FeedBack;
  p_pid->m_SumError += p_pid->m_Errorm;
	 if(p_pid->m_SumError > p_pid->m_SumErrorMax)
        {
           p_pid->m_SumError = p_pid->m_SumErrorMax;
        }
        else if(p_pid->m_SumError < -p_pid->m_SumErrorMax)
        {
            p_pid->m_SumError = -p_pid->m_SumErrorMax;
        }
        else
        {
            p_pid->m_SumError = p_pid->m_SumError;
        }
	p_pid->m_Output = p_pid->m_Kp*p_pid->m_Errorm + p_pid->m_Ki*p_pid->m_SumErrorMax + p_pid->m_Kd*(p_pid->m_ErrorPre - p_pid->m_Errorm);
	if(p_pid->m_Output > p_pid->m_OutputMax)
        {
           p_pid->m_Output = p_pid->m_OutputMax;
        }
        else if(p_pid->m_Output< -p_pid->m_OutputMax)
        {
            p_pid->m_Output = -p_pid->m_OutputMax;
        }
        else
        {
           p_pid->m_Output =p_pid->m_Output;
        }
	p_pid->m_ErrorPre=p_pid->m_Errorm;

}



