/*****************************************************************************
 *   type.h:  Type definition Header file for NXP LPC17xx Family 
 *   Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.25  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#include <stdint.h>

#ifndef __TYPE_H__
#define __TYPE_H__

#ifndef NULL
#define NULL    ((void *)0)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

#ifndef TRUE
#define TRUE    (1)
#endif

#define true 1
#define false 0

typedef unsigned char  uint8;                   // �޷���8λ���ͱ���  
typedef signed   char  int8;                    // �з���8λ���ͱ���  
typedef unsigned short uint16;                  // �޷���16λ���ͱ��� 
typedef signed   short int16;                   // �з���16λ���ͱ��� 
typedef unsigned int   uint32;                  // �޷���32λ���ͱ��� 
typedef signed   int   int32;                   // �з���32λ���ͱ��� 
typedef float          fp32;                    // �����ȸ�������32λ���ȣ� 
typedef double         fp64;                    // ˫���ȸ�������64λ���ȣ� 

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;
typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

#endif  /* __TYPE_H__ */
