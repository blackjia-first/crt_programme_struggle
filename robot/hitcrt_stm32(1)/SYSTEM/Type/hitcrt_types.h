/*******************************************************************
��Ȩ������HITCRT(�����󾺼�������Э��)
�ļ�����HITCRT_TYPES.h
����޸����ڣ�2011.07.07
�汾��1.0
--------------------------------------------------------------------
ģ����������ģ���������õ�����������ͨ�õĽṹ�塣
--------------------------------------------------------------------
�޸ļ�¼��
����        ʱ��            �汾         ˵��
��ΰ        2010.3.5          1.0        �������ļ�
********************************************************************/
#ifndef  __HITCRT_TYPES_H
#define	 __HITCRT_TYPES_H


#define TRUE		(1)
#define FALSE		(0)

#define DEC			(10)
#define HEX			(16)


typedef unsigned char  		UCHAR8;                    /* defined for unsigned 8-bits integer variable 	    �޷���8λ���ͱ���  */
typedef signed   char  		SCHAR8;                    /* defined for signed 8-bits integer variable		�з���8λ���ͱ���  */
typedef unsigned short 		USHORT16;                  /* defined for unsigned 16-bits integer variable 	�޷���16λ���ͱ��� */
typedef signed   short 		SSHORT16;                  /* defined for signed 16-bits integer variable 		�з���16λ���ͱ��� */
typedef unsigned int   		UINT32;                    /* defined for unsigned 32-bits integer variable 	�޷���32λ���ͱ��� */
typedef int   				SINT32;                    /* defined for signed 32-bits integer variable 		�з���32λ���ͱ��� */
typedef float          		FP32;                      /* single precision floating point variable (32bits) �����ȸ�������32λ���ȣ� */
typedef double         		DB64;                      /* double precision floating point variable (64bits) ˫���ȸ�������64λ���ȣ� */


/*********************����**********************/

/*����궨�ṹ��*/
typedef struct
{
	FP32 X;  //������X����λ��mm��
	FP32 Y;  //������Y����λ��mm��
	FP32 Q;  //�����Q����λ��0.1�ȣ�
}ST_POS;



/*�����йؽṹ��*/
typedef struct
{
	FP32 fpVx;     //X�����
	FP32 fpVy;	   //Y�����
	FP32 fpW;      //��ת�ٶ�
	FP32 fpLength; //�������ȣ���λmm��
	FP32 fpalpha;  //������Y��Ƕȣ���λ:���ȣ�
	FP32 prefpalpha;  //������Y��Ƕȣ���λ:���ȣ�
}ST_VECTOR;

/*������̬�ṹ��*/
typedef struct
{
	FP32 fpPosX;  //������X����λ��mm��
	FP32 fpPosY;  //������Y����λ��mm��
	FP32 fpPosQ;  //�����Q����λ��0.1�ȣ�
	FP32 fpPosX1;
	FP32 fpPosY1;
	FP32 fpPosQ1;
}ST_POT;



/*�ٶȽṹ��*/
typedef struct
{
	FP32 fpVx; //�ط����ٶȣ���λmm/s��
	FP32 fpVy; //Y�����ٶȣ���λ��mm/s��
	FP32 fpW;  //���ٶȣ���λ0.1��/s��
}ST_VELT;

#endif
