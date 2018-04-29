//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    KrnlSem.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "KrnlSem.h"
#include "Kernel.h"
#include "SysMod.h"
#include "PCB.h"
#include "schedule.h"
#include "semaphor.h"

void KernelSem::block(){
	BlockedThreads.put(Kernel::block());
	Kernel::dispatch();
}

void KernelSem::deblock(){
	Kernel::deblock(BlockedThreads.get());
	if(preempt) Kernel::dispatch();
}

KernelSem::KernelSem(int init, int pree):
		value(init),
		preempt(pree)
{
	id = Kernel::AllSemaphores.Push(this);
}

KernelSem::~KernelSem(){
	while(! BlockedThreads.IsEmpty())
		signal();
	Kernel::AllSemaphores.Pop(id);
}

void KernelSem::wait(){
	if(--value<0) block();
}

void KernelSem::signal(){
	if(value++<0) deblock();
}
