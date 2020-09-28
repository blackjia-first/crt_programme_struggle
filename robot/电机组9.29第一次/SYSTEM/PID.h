#ifndef __C_PID__
#define __C_PID__

typedef struct pid_t
{

    float m_Des; //目标值
    float m_FeedBack; //反馈值

    float m_Errorm; //偏差值
    float m_ErrorPre; //上次偏差值

    float m_SumError; //偏差值积分
    float m_SumErrorMax; //偏差值上限

    float m_Kp; //P
    float m_Ki; //I
    float m_Kd; //D

    float m_Output; //输出值
    float m_OutputMax; //输出值上限

    
}PID_t;

extern PID_t speed_pid_1,pos_pid_1,speed_pid_2,pos_pid_2;
  

  //构造函数
   extern void pid_init(PID_t * p_pid, float SumErrorMax, float Kp, float Ki, float Kd, float OutputMax);
    //PID对象获取反馈值
	 extern   void SetFeedBack(PID_t * p_pid,float FeedBack);

    //PID对象设置目标值
	 extern   void SetDes(PID_t * p_pid,float Des);

    //PID计算
	 extern   void Calculate(PID_t *p_pid);

#endif
