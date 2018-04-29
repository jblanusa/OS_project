//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    SysMod.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef OSZAD2_H_SYSMODE_H_
#define OSZAD2_H_SYSMODE_H_

#include "define.h"
#include "PCB.h"
#include "Thread.h"

typedef enum {CREATE_THREAD, DELETE_THREAD, START_THREAD, WAIT_THREAD, SLEEP_THREAD, FINISH_THREAD,MAKE_SEM,
	DELETE_SEM, SIGNAL_SEM, WAIT_SEM, VAL_SEM, CREATE_EVENT, DELETE_EVENT, SIGNAL_EVENT, WAIT_EVENT} SysCallID;
class SysMode{
private:

	static pInterrupt OldSwitchToKernel;
	static pInterrupt OldSwitchToUser;

	static PCB* kernelPCB;
	static unsigned* userStack;

	static int i;
	struct Data{
		ID* targetID;
		StackSize sstack;
		Time timeData;
		int* val;
		IVTNo no;
		SysCallID sysID;
		Data(SysCallID sid = -1, ID* i = 0, StackSize s = -1, Time t = -1, int* v = 0, IVTNo n = -1):targetID(i),sstack(s),timeData(t),val(v),no(n),sysID(sid){}
		Data(SysCallID sid, ID* i, Time t):targetID(i),sstack(-1),timeData(t),val(0),no(-1),sysID(sid){}
		Data(SysCallID sid, ID* i, int* v):targetID(i),sstack(-1),timeData(-1),val(v),no(-1),sysID(sid){}
	};
public:
	static int sysmode;

	static void interrupt SwitchToKernel(...);
	static void interrupt SwitchToUser(...);

	static void initSys();
	static void restoreSys();

	static void SysCall(Data* a);

//********SISTEMSKI POZIVI***********
//Thread
	static void CreateThread(StackSize, Time, Thread*);
	static void DeleteThread(ID);
	static void StartThread(ID);
	static void WaitThread(ID);
	static void FinishThread(ID);
	static void SleepThread(ID,Time);
//Semaphore
	static void MakeSem(ID*,int);
	static void DeleteSem(ID);
	static void SignalSem(ID);
	static void WaitSem(ID);
	static int ValSem(ID);
//Event
	static void CreateEvent(ID*,IVTNo);
	static void DeleteEvent(ID);
	static void SignalEvent(ID);
	static void WaitEvent(ID);
};

#endif
