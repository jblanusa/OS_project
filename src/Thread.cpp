//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    Thread.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "thread.h"
#include "SysMod.h"

void Thread::waitToComplete(){
	SysMode::WaitThread(myPCBID);
}

void Thread::start() {
	SysMode::StartThread(myPCBID);
}

Thread::Thread (StackSize stackSize, Time timeSlice):myPCBID(-1)
{
	SysMode::CreateThread(stackSize,timeSlice,this);
}

void Thread::sleep(Time timeToSleep){
	SysMode::SleepThread(Kernel::running->id,timeToSleep);
}

Thread::~Thread(){
	this->waitToComplete();
	SysMode::DeleteThread(myPCBID);
}

void dispatch(){
	Kernel::dispatch();
}

void Thread::wrapper(Thread* running){
	running->run();
	SysMode::FinishThread(running->myPCBID);
}
