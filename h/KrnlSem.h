//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    KrnlSem.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _KERNELSEM_H_
#define _KERNELSEM_H_

#include "define.h"
#include "queue.h"

class PCB;
class Semaphore;

class KernelSem {
public:
	KernelSem (int init = 1, int preempt = 0);
	 ~KernelSem();

	void wait();
	void signal();
	int Val(){return value;}
protected:
	friend class Semaphore;
	friend class KernelEv;
	friend class SysMode;

	void block();
	void deblock();
private:
	ID id;
	int value;
	int preempt;
	Queue<PCB*> BlockedThreads;
};


#endif
