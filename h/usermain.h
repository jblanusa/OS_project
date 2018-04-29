//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    usermain.h
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#ifndef _umain_h_
#define _umain_h_

#include "thread.h"

class UserMain: public Thread {
public:
	UserMain(int, char **);
	~UserMain();
	void run();

	int getReturn() const;
protected:
	friend class Kernel;
	int ret;

private:
	int argc;
	char **argv;

};

#endif
