#include "moduleFoo.h"

#include <iostream>

int
main()
{
	std::cout << "#### Foo\n";

	ObjectFoo f = makeFoo(10);
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';

	std::cout << "\tPerform wibble-zorbling\n";
	f.wibble();
	f.zorble();

	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';

	std::cout << '\n';
	return 0;
}
