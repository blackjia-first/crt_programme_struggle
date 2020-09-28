/*******************************************************************
版权声明：HITCRT(哈工大竞技机器人协会)
文件名：HITCRT_TYPES.h
最近修改日期：2011.07.07
版本：1.0
--------------------------------------------------------------------
模块描述：该模块申明常用的数据类型与通用的结构体。
--------------------------------------------------------------------
修改记录：
作者        时间            版本         说明
任伟        2010.3.5          1.0        建立此文件
********************************************************************/
#ifndef  __HITCRT_TYPES_H
#define	 __HITCRT_TYPES_H


#define TRUE		(1)
#define FALSE		(0)

#define DEC			(10)
#define HEX			(16)


typedef unsigned char  		UCHAR8;                    /* defined for unsigned 8-bits integer variable 	    无符号8位整型变量  */
typedef signed   char  		SCHAR8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short 		USHORT16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short 		SSHORT16;                  /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   		UINT32;                    /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef int   				SINT32;                    /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          		FP32;                      /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         		DB64;                      /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */


/*********************向量**********************/

/*坐标标定结构体*/
typedef struct
{
	FP32 X;  //横坐标X（单位：mm）
	FP32 Y;  //竖坐标Y（单位：mm）
	FP32 Q;  //航向角Q（单位：0.1度）
}ST_POS;



/*向量有关结构体*/
typedef struct
{
	FP32 fpVx;     //X方向差
	FP32 fpVy;	   //Y方向差
	FP32 fpW;      //旋转速度
	FP32 fpLength; //向量长度（单位mm）
	FP32 fpalpha;  //向量与Y轴角度（单位:弧度）
	FP32 prefpalpha;  //向量与Y轴角度（单位:弧度）
}ST_VECTOR;

/*坐标姿态结构体*/
typedef struct
{
	FP32 fpPosX;  //横坐标X（单位：mm）
	FP32 fpPosY;  //竖坐标Y（单位：mm）
	FP32 fpPosQ;  //航向角Q（单位：0.1度）
	FP32 fpPosX1;
	FP32 fpPosY1;
	FP32 fpPosQ1;
}ST_POT;



/*速度结构体*/
typedef struct
{
	FP32 fpVx; //Ｘ方向速度（单位mm/s）
	FP32 fpVy; //Y方向速度（单位：mm/s）
	FP32 fpW;  //角速度（单位0.1度/s）
}ST_VELT;

#endif
