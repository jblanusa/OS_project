//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    IVTentry.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _IVTENTRY_H_
#define _IVTENTRY_H_

#include "define.h"

#define PREPAREENTRY(no,callOld)          \
	void interrupt EventRoutine##no(...);        \
	IVTEntry ivtEntry##no(no,EventRoutine##no);  \
	void interrupt EventRoutine##no(...){        \
		ivtEntry##no.signal();             \
 		if (callOld) ivtEntry##no.OldInterrupt();  \
	}


class IVTEntry {
public:
	IVTEntry(IVTNo, pInterrupt);
	~IVTEntry();

	pInterrupt OldInterrupt;
	void signal();
private:
	IVTNo ivt;
};


#endif
