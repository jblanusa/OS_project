//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    PCB.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef OSZAD2_H_PCB_H_
#define OSZAD2_H_PCB_H_

#include "thread.h"
#include "KrnlSem.h"

class PCB{
public:
	void waitToComplete();
	void waitForChildren();

	char* ime;
private:
	static void FinishThread(ID);
	static void sleep(ID,Time timeToSleep);

	void start();
	void allocateContext();

	//Processes context
	unsigned sss, ssp, sbp;
	unsigned* stack;
	unsigned stackSize;

	Time slice;
	Time sleepTime;

	Thread* myThread;
	PCB* myParent;

	KernelSem waitChildren;
	unsigned numOfChildren;
	KernelSem waitComplete;
	unsigned numOfWaits;

	enum State { NEW, READY, RUNNING, BLOCKED, TERMINATED, KERNEL } state;
	ID id;
protected:
	PCB(StackSize stackSize  = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread* thread = 0);
	~PCB();


	friend class Thread;
	friend class Kernel;
	friend class SysMode;
	friend class TimedQueue;
};


#endif /* OSZAD2_H_PCB_H_ */
