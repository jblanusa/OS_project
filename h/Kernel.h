//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    Kernel.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef OSZAD2_H_KERNEL_H_
#define OSZAD2_H_KERNEL_H_

#include "define.h"
#include "vector.h"

class Idle;
class PCB;
class KernelSem;
class KernelEv;
class Thread;
class TimedQueue;
class UserMain;

class Kernel {
public:
	static PCB* running;
	static PCB* Main;
	static Idle* idleThread;
	static UserMain* User;

	static void interrupt dispatch(...);
	static void interrupt TimerInterrupt(...);
	static void ContextSwitch();

	static PCB* block();
	static void deblock(PCB*);

	static void init();
	static void kill();

	static void lock();
	static void unlock();

protected:
	friend class PCB;
	friend class Idle;
	friend class Thread;
	friend class KernelSem;
	friend class Semaphore;
	friend class KernelEv;
	friend class Event;
	friend class IVTEntry;
	friend class UserMain;
	friend class SysMode;

private:
	static void initTimer();
	static void restoreTimer();
	static volatile Time counter;

	static TimedQueue* SleepyThreads;
	static KernelEv* EventEntries[ENTRIES];
	static void InitEntries();
	static void KillEntries();

	static volatile unsigned lockFlag; // 0 - unlocked, 1 - locked
	static volatile unsigned dispatchRequest;
	static pInterrupt OldTimerInterrupt;
public: //Treba da bude private
	static Vector<PCB> AllPCBs;
	static Vector<KernelSem> AllSemaphores;
	static Vector<KernelEv> AllEvents;
};




#endif /* OSZAD2_H_KERNEL_H_ */
