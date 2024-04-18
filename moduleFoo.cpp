#include "moduleFoo.h"

void
ObjectFoo::zorble()
{
	myZorble = 'a' + (myZorble - 'a' + 1) % ('z' - 'a' + 1);
}

std::string
ObjectFoo::toString(uint8_t tabs) const
{
	std::string tab;
	for (uint8_t i = 0; i < tabs; ++i)
		tab += '\t';

	std::string res;
	res += tab + "wibble: " + std::to_string(myWibble) + '\n';
	res += tab + "zorble: " + myZorble + '\n';
	if (myFlibbers.size() > 0)
	{
		res += tab + "flibbers:\n";
		for (const Flibber& fli : myFlibbers)
			res += tab + '\t' + fli.myKey + ": " + fli.myValue + '\n';
	}
	return res;
}

ObjectFoo
makeFoo(int wibble)
{
	ObjectFoo f;
	f.myWibble = wibble;
	return f;
}
