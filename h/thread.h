//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    thread.h
//   Student: Jovan Blanu�a 47/2012
//===========================================================================//
#ifndef _thread_h_
#define _thread_h_

#include "Kernel.h"

typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; //time, x 55ms
const Time defaultTimeSlice = 2; // default = 2 * 55ms
typedef int ID;

class PCB; //Kernel's implementation of a user's thread

class Thread {
public:
	void start();
	void waitToComplete();
	virtual ~Thread();

	static void sleep(Time timeToSleep);
protected:
	friend class PCB;

	Thread(StackSize stackSize = defaultStackSize, Time timeSlice = defaultTimeSlice);
	virtual void run() {}
//private:
public:
	ID myPCBID;

	static void wrapper(Thread* running);
};

void dispatch();


#endif /* H_THREAD_H_ */
