#include "KrnlSem.h"
#include "Kernel.h"
#include "SysMod.h"
#include "semaphor.h"

Semaphore::Semaphore(int init)
{
	SysMode::MakeSem(&mySemID,init);
}

Semaphore::~Semaphore(){
	SysMode::DeleteSem(mySemID);
}


int Semaphore::val() const{
	return SysMode::ValSem(mySemID);
}

void Semaphore::signal(){
	SysMode::SignalSem(mySemID);
}

void Semaphore::wait(){
	SysMode::WaitSem(mySemID);
}
