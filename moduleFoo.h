#pragma once

#include "core.h"

#include <vector>

struct ObjectFoo
{
	void wibble() { ++myWibble; }
	void zorble();
	void flibber();

	int myWibble = 0;
	char myZorble = 'a';
	std::vector<Flibber> myFlibbers;
};

ObjectFoo makeFoo(int wibble);
