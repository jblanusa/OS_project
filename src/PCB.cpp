//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    PCB.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "PCB.h"
#include "Kernel.h"
#include "schedule.h"
#include "thread.h"
#include "TQueue.h"
#include <dos.h>

PCB::PCB(StackSize ssize, Time tslice, Thread* thread):
		stackSize(ssize),
		slice(tslice),
		myThread(thread),
		numOfChildren(0),
		myParent(Kernel::running),
		numOfWaits(0),
		stack(0),
		sss(0),
		ssp(0),
		sbp(0),
		sleepTime(0),
		waitChildren(0),
		waitComplete(0),
		ime("Neimenovan"),
		id(-1)
{
	id = Kernel::AllPCBs.Push(this);
	if(thread) thread->myPCBID = id;
	if(myParent) myParent->numOfChildren++;
	state = NEW;
}

PCB::~PCB(){
	Kernel::AllPCBs.Pop(id);
	delete stack;
}

void PCB::sleep(ID pcbID, Time timeToSleep){
	Kernel::AllPCBs[pcbID]->sleepTime = timeToSleep;
	Kernel::AllPCBs[pcbID]->state = PCB::BLOCKED;
	Kernel::SleepyThreads->put(Kernel::AllPCBs[pcbID]);
	Kernel::dispatch();
}

void PCB::start(){
	state = PCB::READY;
	allocateContext();
	Scheduler::put(this);
}

void PCB::waitForChildren(){
	Kernel::lock();
	for(int i = 0; i < numOfChildren; i++){
		waitChildren.wait();
	}
	numOfChildren = 0;
	Kernel::unlock();
}

void PCB::waitToComplete(){
	if(this == Kernel::running || state == TERMINATED){
		return;
	}
	numOfWaits++;
	waitComplete.wait();
}

void PCB::allocateContext(){
	stack = new unsigned[stackSize];
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize-1] = FP_SEG(myThread);
	stack[stackSize-2] = FP_OFF(myThread);
#endif
	stack[stackSize-5] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[stackSize-6] = FP_SEG(Thread::wrapper);
	stack[stackSize-7] = FP_OFF(Thread::wrapper);
	sss = FP_SEG(stack + stackSize - 16);
	ssp = FP_OFF(stack + stackSize - 16);
	sbp = FP_OFF(stack + stackSize - 16);
#endif
}

void PCB::FinishThread(ID pcbID){
	for(int i = 0; i < Kernel::AllPCBs[pcbID]->numOfWaits; i++){
		Kernel::AllPCBs[pcbID]->waitComplete.signal();
	}

	if (Kernel::AllPCBs[pcbID]->myParent) {
		Kernel::AllPCBs[pcbID]->myParent->waitChildren.signal();
	}

	Kernel::AllPCBs[pcbID]->state = PCB::TERMINATED;
	Kernel::dispatch();
}
