//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    semaphor.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _semaphor_h_
#define _semaphor_h_

#include "define.h"


class Semaphore {
public:
	Semaphore (int init = 1);
	virtual ~Semaphore();

	virtual void wait  ();
	virtual void signal();

	int val() const; //Returns the current value of the semaphore

private:
	ID mySemID;
};



#endif /* H_SEMAPHOR_H_ */
