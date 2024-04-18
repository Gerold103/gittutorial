#include "moduleFoo.h"

void
ObjectFoo::zorble()
{
	myZorble = 'a' + (myZorble - 'a' + 1) % ('z' - 'a' + 1);
}

void
ObjectFoo::flibber()
{
	myFlibbers.push_back(makeFlibber());
}

ObjectFoo
makeFoo()
{
	return ObjectFoo();
}
