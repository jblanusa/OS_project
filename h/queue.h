//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    queue.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef OSZAD2_H_QUEUE_H_
#define OSZAD2_H_QUEUE_H_

#include <iostream.h>

template<class T>
class Queue{
public:
	Queue():first(0),last(0),length(0){}

	virtual ~Queue(){
		kill();
		first = 0;
		last = 0;
	}


	T get(){
		if(!first) return 0;

		T rez = first->curr;
		elem* temp = first;
		first = first->next;
		length--;

		delete temp;
		return rez;
	}

	void put(T t){
		elem* e = new elem(t,0);

		length++;
		if(!first){
			first = last = e;
			return;
		}

		last->next = e;
		last = last->next;
	}

	int IsEmpty(){
		return length==0?1:0;
	}

//Metode koje vektor koristi
	void PriorityPut(T t){
		elem* e = new elem(t,0);

		length++;
		if(!first){
			first = last = e;
			return;
		}

		elem* temp = first;
		elem* prev = 0;
		while(temp && temp->curr < t){
				prev = temp;
				temp = temp->next;
		}

		e->next = temp;
		if(prev) prev->next = e; else first = e;
		if(!temp) last = e;
	}

private:
	struct elem{
		elem(T c, elem* n):curr(c),next(n){}
		T curr;
		elem* next;
	};

	void kill(){
		while(first){
			elem* temp = first;
			first = first->next;
			length--;
			delete temp;
		}

	};

	unsigned length;
	elem* first;
	elem* last;
};



#endif /* OSZAD2_H_QUEUE_H_ */
