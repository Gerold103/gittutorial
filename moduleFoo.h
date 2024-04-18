#pragma once

struct ObjectFoo
{
	void wibble() { ++myWibble; }
	void zorble();

	int myWibble = 0;
	char myZorble = 'a';
};

ObjectFoo makeFoo(int wibble);
