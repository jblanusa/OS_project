//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    define.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _DEFINE_H_
#define _DEFINE_H_

#define TIMER_ENTRY 0x08
#define SWKERNEL_ENTRY 0x60
#define SWUSER_ENTRY 0x61
#define ENTRIES 256

typedef unsigned long StackSize;
typedef unsigned int Time; //time, x 55ms
typedef int ID;
typedef void interrupt (*pInterrupt)(...);
typedef unsigned char IVTNo;

#endif

