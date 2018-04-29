//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    TQueue.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "TQueue.h"
#include "PCB.h"

void TimedQueue::put(PCB* pcb){
	elem* e = new elem(pcb,0);
	length++;

	if(!first){
		first = last = e;
		return;
	}

	elem *temp = first, *prev = 0;

	while(temp && pcb->sleepTime >= temp->curr->sleepTime){
		pcb->sleepTime -= temp->curr->sleepTime;
		prev = temp;
		temp = temp->next;
	}

	e->next = temp;
	if(temp) temp->curr->sleepTime -= e->curr->sleepTime;
	if(prev) prev->next = e; else first = e;
	if(!temp) last = e;

}

void TimedQueue::TimerNotification(){
	if(!first) return;
	if(first->curr->sleepTime > 0){
		first->curr->sleepTime--;
	}

	while(first && first->curr->sleepTime == 0){
		Kernel::deblock(first->curr);
		elem* temp = first;
		first = first->next;
		delete temp;
	}
}

void TimedQueue::kill(){
	while(first){
		elem* temp = first;
		first = first->next;
		length--;
		delete temp;
	}
}
