//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    IVTentry.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include <dos.h>
#include "IVTentry.h"
#include "KrnlEv.h"
#include "Kernel.h"

IVTEntry::IVTEntry(IVTNo ivtNo, pInterrupt eventRoutine):
	ivt(ivtNo),
	OldInterrupt(0)
{
#ifndef BCC_BLOCK_IGNORE
	OldInterrupt = getvect(ivtNo);
	setvect(ivtNo, eventRoutine);
#endif
}

IVTEntry::~IVTEntry() {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivt, OldInterrupt);
#endif
}

void IVTEntry::signal(){
	if(Kernel::EventEntries[ivt]) Kernel::EventEntries[ivt]->signal();
}
