//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    event.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _event_h_
#define _event_h_

#include "define.h"
#include "IVTentry.h"

typedef unsigned char IVTNo;

class Event {
public:
	Event(IVTNo ivtNo);
   ~Event ();

   void wait();

protected:
   friend class KernelEv;

public:
   void signal(); // can call KernelEv

private:
   ID myEvID;
};



#endif /* H_EVENT_H_ */
