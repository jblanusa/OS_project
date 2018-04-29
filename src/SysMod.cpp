//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    SysMod.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "SysMod.h"
#include "KrnlSem.h"
#include "KrnlEv.h"
#include <dos.h>

#define KERN_SIZE 1024
#define TOP 18

pInterrupt SysMode::OldSwitchToKernel;
pInterrupt SysMode::OldSwitchToUser;

PCB* SysMode::kernelPCB = 0;
unsigned* SysMode::userStack = 0;

int SysMode::i = 0;
int SysMode::sysmode = 0;


void SysMode::initSys(){
	kernelPCB = new PCB(KERN_SIZE,0,0);
	kernelPCB->state = PCB::KERNEL;
	kernelPCB->allocateContext(); // Da li treba uopste?
#ifndef BCC_BLOCK_IGNORE
	OldSwitchToKernel = getvect(SWKERNEL_ENTRY);
	OldSwitchToUser = getvect(SWUSER_ENTRY);
	setvect(SWKERNEL_ENTRY, SwitchToKernel);
	setvect(SWUSER_ENTRY, SwitchToUser);
#endif
}

void SysMode::restoreSys(){
#ifndef BCC_BLOCK_IGNORE
	setvect(SWKERNEL_ENTRY, OldSwitchToKernel);
	setvect(SWUSER_ENTRY, OldSwitchToUser);
#endif
}

void interrupt SysMode::SwitchToKernel(...){
#ifndef BCC_BLOCK_IGNORE
	Kernel::running->ssp = _SP;
	Kernel::running->sss = _SS;
	userStack = (unsigned*)MK_FP(Kernel::running->sss, Kernel::running->ssp);
#endif
	sysmode = 1;
	for (i = 0; i < TOP; i++) kernelPCB->stack[KERN_SIZE - 1 - i] = userStack[TOP - 1 - i];

#ifndef BCC_BLOCK_IGNORE
	kernelPCB->stack[KERN_SIZE - TOP] = FP_OFF(kernelPCB->stack + KERN_SIZE - 5); //BP = oldBP
	_SS = FP_SEG(kernelPCB->stack + KERN_SIZE - TOP);
	_SP = FP_OFF(kernelPCB->stack + KERN_SIZE - TOP);
	_BP = FP_OFF(kernelPCB->stack + KERN_SIZE - TOP);
#endif
}

void interrupt SysMode::SwitchToUser(...){
	for (i = 0; i < TOP; i ++) userStack[TOP - 1 - i] = kernelPCB->stack[KERN_SIZE - 1 - i];
	sysmode = 0;
#ifndef BCC_BLOCK_IGNORE
	_SS = Kernel::running->sss;
	_SP = Kernel::running->ssp;
	_BP = Kernel::running->sbp;
#endif
}

void SysMode::SysCall(Data* d){
//	_BX = FP_SEG(a);
//	_CX = FP_OFF(a);
	asm int 60h;
	switch(d->sysID)
	{
	   case WAIT_THREAD :
		   Kernel::AllPCBs[*d->targetID]->waitToComplete();
		   break;
	   case START_THREAD :
		   Kernel::AllPCBs[*d->targetID]->start();
		   break;
	   case CREATE_THREAD :
		   new PCB(d->sstack,d->timeData,(Thread*)d->val);
		   break;
	   case DELETE_THREAD :
		   if(Kernel::AllPCBs[*d->targetID]) delete Kernel::AllPCBs[*d->targetID];
		   break;
	   case FINISH_THREAD :
		   PCB::FinishThread(*d->targetID);
		   break;
	   case SLEEP_THREAD :
		   PCB::sleep(*d->targetID,d->timeData);
		   break;
	   case DELETE_SEM :
		   if(Kernel::AllSemaphores[*d->targetID]) delete Kernel::AllSemaphores[*d->targetID];
		   break;
	   case SIGNAL_SEM :
		   Kernel::AllSemaphores[*d->targetID]->signal();
		   break;
	   case WAIT_SEM :
		   Kernel::AllSemaphores[*d->targetID]->wait();
		   break;
	   case VAL_SEM:
		   *d->val = Kernel::AllSemaphores[*d->targetID]->Val();
		   break;
	   case CREATE_EVENT :
		   *d->targetID = (new KernelEv(d->no))->id;
		   break;
	   case DELETE_EVENT :
		   if(Kernel::AllEvents[*d->targetID]) delete Kernel::AllEvents[*d->targetID];
		   break;
	   case SIGNAL_EVENT :
		   Kernel::AllEvents[*d->targetID]->signal();
		   break;
	   case WAIT_EVENT :
		   Kernel::AllEvents[*d->targetID]->wait();
		   break;
	   case MAKE_SEM :
		   *d->targetID = (new KernelSem(*d->val))->id;
		   break;
	   default:;
	 }
	asm int 61h;
}

//**************************************************************************************
//***************************** SISTEMSKI POZIVI ***************************************
//**************************************************************************************
// * * * THREAD * * *

void SysMode::StartThread(ID pcbID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(START_THREAD,&pcbID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::WaitThread(ID pcbID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(WAIT_THREAD,&pcbID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::CreateThread(StackSize ssize, Time slice, Thread* th){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(CREATE_THREAD,0,ssize,slice,(int*)th);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::DeleteThread(ID pcbID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(DELETE_THREAD,&pcbID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::FinishThread(ID pcbID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(FINISH_THREAD,&pcbID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::SleepThread(ID pcbID, Time toSleep){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(SLEEP_THREAD,&pcbID, toSleep);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

// * * * SEMAPHORE * * *

void SysMode::DeleteSem(ID semID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(DELETE_SEM,&semID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::SignalSem(ID semID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(SIGNAL_SEM,&semID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::WaitSem(ID semID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(WAIT_SEM,&semID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

int SysMode::ValSem(ID semID){
	Kernel::lock();
	int ret;
	SysMode::Data *d = new SysMode::Data(VAL_SEM,&semID, -1, -1, &ret);
	SysCall(d);
	delete d;
	Kernel::unlock();
	return ret;
}

void SysMode::MakeSem(ID *semID, int init){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(MAKE_SEM, semID, -1, -1, &init);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

// * * * EVENT * * *
void SysMode::CreateEvent(ID *evID, IVTNo iv){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(CREATE_EVENT, evID, -1, -1, 0, iv);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::DeleteEvent(ID evID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(DELETE_EVENT,&evID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::SignalEvent(ID evID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(SIGNAL_EVENT,&evID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}

void SysMode::WaitEvent(ID evID){
	Kernel::lock();
	SysMode::Data *d = new SysMode::Data(WAIT_EVENT,&evID);
	SysCall(d);
	delete d;
	Kernel::unlock();
}
