#include "moduleFoo.h"

void
ObjectFoo::zorble()
{
	myZorble = 'a' + (myZorble - 'a' + 1) % ('z' - 'a' + 1);
}

ObjectFoo
makeFoo(int wibble)
{
	ObjectFoo f;
	f.myWibble = wibble;
	return f;
}
