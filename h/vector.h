//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    vector.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef OSZAD2_VECTOR_H_
#define OSZAD2_VECTOR_H_

#include "define.h"
#include "queue.h"

#define BLOCK_SIZE 200

template <class T>
class Vector {
public:
	Vector():
		cnt(0),
		size(BLOCK_SIZE),
		first(0),
		last(0),
		length(0)
	{

	}

	~Vector() {
		kill();
	}

	 int Push(T* t) {
		if(cnt == length * BLOCK_SIZE) allocate();

		cnt++;
		int id = freeID.get();
		getElemByID(id) = t;
		return id;
	}

	void Pop(ID id) {
		getElemByID(id) = 0;
		freeID.PriorityPut(id);
		cnt --;

	}

	T*& operator[](ID i) {
		return getElemByID(i);
	}

private:
	unsigned cnt, size;
	Queue<ID> freeID;

	struct elem{
		T* x[BLOCK_SIZE];
		elem* next;
		elem(elem* n):next(n){}
	};

	elem *first;
	elem *last;
	unsigned length;

	void allocate(){
		elem* temp = new elem(0);

		length++;
		size += BLOCK_SIZE;


		for (ID i = 0; i < BLOCK_SIZE; i++) {
			temp->x[i] = 0;
			freeID.PriorityPut(i + (length-1) * BLOCK_SIZE);
		}

		if(!first){
			first = last = temp;
			return;
		}

		last->next = temp;
		last = temp;
	}


//	 void deallocate(){
//	 	 ...
//	 	 }


	void kill(){
		while(first){
			elem* temp = first;
			first = first->next;
			length--;
			delete temp;
		}
	}

	T*& getElemByID(ID i){
		elem* temp = first;
		for(int j = 0; j < i/BLOCK_SIZE; j++){
			temp = temp->next;
		}
		return temp->x[i];
	}
};

#endif
