//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    KrnlEv.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef H_KEVENT_H_
#define H_KEVENT_H_

#include "define.h"
#include "KrnlSem.h"

class PCB;
class Event;

class KernelEv {
public:
	KernelEv(IVTNo);
   ~KernelEv ();

   void wait();
protected:
   friend class Event;
   friend class SysMode;
   friend class IVTEntry;

   void signal();

private:
   ID id;
   IVTNo ivt;
   PCB* owner;
   KernelSem mySem, mutex;

};



#endif /* H_EVENT_H_ */
