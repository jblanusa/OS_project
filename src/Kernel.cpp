//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    Kernel.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "Kernel.h"
#include "PCB.h"
#include "schedule.h"
#include "TQueue.h"
#include "idle.h"
#include "KrnlEv.h"
#include "SysMod.h"
#include <dos.h>

PCB* Kernel::running = 0;
PCB* Kernel::Main = 0;
Idle* Kernel::idleThread = 0;
UserMain* Kernel::User = 0;

volatile Time Kernel::counter = 0;
pInterrupt Kernel::OldTimerInterrupt = 0;

volatile unsigned Kernel::lockFlag = 0;
volatile unsigned Kernel::dispatchRequest = 0;

TimedQueue* Kernel::SleepyThreads = 0;
KernelEv* Kernel::EventEntries[ENTRIES] = {0};

Vector<PCB> Kernel::AllPCBs;
Vector<KernelSem> Kernel::AllSemaphores;
Vector<KernelEv> Kernel::AllEvents;

void Kernel::ContextSwitch(){
	if ( Kernel::running ) {
		if (Kernel::running->state == PCB::RUNNING) {
			Kernel::running->state = PCB::READY;
			Scheduler::put(Kernel::running);
		}
	}
	Kernel::running = Scheduler::get();
	if (Kernel::running) { Kernel::running->state = PCB::RUNNING; }
}

void interrupt Kernel::dispatch(...) {
if(!lockFlag){
	dispatchRequest = 0;
	if ( Kernel::running ) {
		if(!SysMode::sysmode){
#ifndef BCC_BLOCK_IGNORE

		Kernel::running->ssp = _SP;
		Kernel::running->sss = _SS;
		Kernel::running->sbp = _BP;
#endif
		}

		if (Kernel::running->state == PCB::RUNNING) {
			Kernel::running->state = PCB::READY;
			Scheduler::put(Kernel::running);
		}
	}

	Kernel::running = Scheduler::get();
	if (Kernel::running) { Kernel::running->state = PCB::RUNNING; }

	if(!SysMode::sysmode){
#ifndef BCC_BLOCK_IGNORE
		_SP = Kernel::running->ssp;
		_SS = Kernel::running->sss;
		_BP = Kernel::running->sbp;
#endif
		}
	}
	else dispatchRequest = 1;
	counter = 0;
}

void Kernel::init(){
	Kernel::idleThread = new Idle();
	Kernel::Main = new PCB();
	Kernel::SleepyThreads = new TimedQueue();
	Kernel::running = Kernel::Main;
	Kernel::Main->ime = "Main";
	Kernel::running->state = PCB::RUNNING;
	Kernel::InitEntries();
	initTimer();
}

void Kernel::kill(){
	Kernel::Main->waitForChildren();
	Kernel::lock();
	if(Kernel::SleepyThreads) delete Kernel::SleepyThreads;
	if(Kernel::idleThread) delete Kernel::idleThread;
	if(Kernel::Main) delete Kernel::Main;
	Kernel::KillEntries();
	Kernel::unlock();
	restoreTimer();
}

void Kernel::initTimer(){
#ifndef BCC_BLOCK_IGNORE
	OldTimerInterrupt = getvect(TIMER_ENTRY);
	setvect(TIMER_ENTRY, TimerInterrupt);
#endif
}

void Kernel::restoreTimer(){
#ifndef BCC_BLOCK_IGNORE
	setvect(TIMER_ENTRY, OldTimerInterrupt);
#endif
}

// Ovu funkciju treba da definise korisnicki program
extern void tick();

void interrupt Kernel::TimerInterrupt(...) {
	counter++;
	OldTimerInterrupt();

	tick();
	Kernel::SleepyThreads->TimerNotification();
	if (running->slice != 0 && counter > running->slice) {
		Kernel::dispatch();
		counter = 0;
	}
}

void Kernel::lock(){
	Kernel::lockFlag = 1;
}

void Kernel::unlock(){
	Kernel::lockFlag = 0;
	if(Kernel::dispatchRequest) Kernel::dispatch();
}

void Kernel::deblock(PCB* pcb){
	if(pcb && pcb != Kernel::running && pcb->state == PCB::BLOCKED){
		pcb->state = PCB::READY;
		Scheduler::put(pcb);
	}
}

PCB* Kernel::block(){
	Kernel::running->state = PCB::BLOCKED;
	return Kernel::running;

}

void Kernel::InitEntries(){
	for(int i = 0; i < ENTRIES; i++){
		Kernel::EventEntries[i] = 0;
	}
}

void Kernel::KillEntries(){
	for(int i = 0; i < ENTRIES; i++){
		if(Kernel::EventEntries[i]) delete Kernel::EventEntries[i];
		Kernel::EventEntries[i] = 0;
	}
}
