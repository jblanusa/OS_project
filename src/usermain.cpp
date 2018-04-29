//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    usermain.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "usermain.h"
#include "thread.h"
#include "PCB.h"

extern int userMain(int, char**);

UserMain::UserMain(int argcc, char *argvv[]):
	argc(argcc),
	argv(argvv),
	ret(0)
{
	Kernel::AllPCBs[myPCBID]->ime = "UserMain";
}

void UserMain::run() {
	ret = userMain(argc, argv);
}

int UserMain::getReturn() const{
	return ret;
}

UserMain::~UserMain(){
	waitToComplete();
}
