//===========================================================================//
//   Project: Projekat iz Operativnih sistema 1
//   File:    main.cpp
//   Student: Jovan Blanuša 47/2012
//===========================================================================//
#include "usermain.h"
#include "Kernel.h"
#include "SysMod.h"
#include <dos.h>

int main(int argc, char *argv[]){
	SysMode::initSys();
	Kernel::init();

	UserMain* user = new UserMain(argc, argv);
	user->start();
	user->waitToComplete();
	int ret = user->getReturn();

	Kernel::kill();
	delete user;
	SysMode::restoreSys();

	return ret;
}




