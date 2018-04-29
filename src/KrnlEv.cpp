//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    KrnlEv.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "KrnlEv.h"
#include "SysMod.h"
#include "event.h"
#include "semaphor.h"
#include "Kernel.h"

KernelEv::KernelEv(IVTNo no):
	ivt(no),
	mySem(0,1),
	mutex(1),
	owner(Kernel::running)
{
	Kernel::EventEntries[ivt] = this;
	id = Kernel::AllEvents.Push(this);
}

KernelEv::~KernelEv(){
	Kernel::EventEntries[ivt] = 0;
	Kernel::AllEvents.Pop(id);
}

void KernelEv::wait(){
	if(owner == Kernel::running)
	{
		mySem.wait();
	}
}

void KernelEv::signal(){
	mutex.wait();
	if (mySem.value<1) mySem.signal();
	mutex.signal();
}

//------------------------------------------------
//----------------- EVENT --------------------
//------------------------------------------------
Event::Event(IVTNo no){
	SysMode::CreateEvent(&myEvID,no);
}

Event::~Event(){
	SysMode::DeleteEvent(myEvID);
}

void Event::wait(){
	SysMode::WaitEvent(myEvID);
}

void Event::signal(){
	SysMode::SignalEvent(myEvID);
}
