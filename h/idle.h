//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    idle.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _IDLE_H_
#define _IDLE_H_

#include "thread.h"
#include "TQueue.h"

class Idle : public Thread {
public:
	Idle():end(0){ Kernel::AllPCBs[myPCBID]->ime = "IDLE"; Thread::start();};
	~Idle(){ end = 1; };

protected:
	friend class Kernel;
	friend class PCB;
	void run(){ while(!end); }

private:
	int end;

};

#endif
