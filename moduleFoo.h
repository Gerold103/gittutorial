#pragma once

#include "core.h"

#include <vector>

struct ObjectFoo : public FlibberCollection
{
	void wibble() { ++myWibble; }
	void unwibble() { --myWibble; }
	void zorble();

	int myWibble = 0;
	char myZorble = 'a';
};

ObjectFoo makeFoo();
