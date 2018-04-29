//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    TQueue.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _TQUEUE_H_
#define _TQUEUE_H_

#include "define.h"

class PCB;

class TimedQueue {
public:
	TimedQueue():first(0),last(0),length(0){};
	~TimedQueue(){kill();};

	void put(PCB*);
protected:
	friend class PCB;
	friend class Kernel;


private:
	void TimerNotification();
	void kill();

	struct elem{
		elem(PCB* c, elem* n):curr(c),next(n){}
		PCB* curr;
		elem* next;
	};

	unsigned length;
	elem* first;
	elem* last;
};


#endif
