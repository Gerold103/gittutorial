#include "moduleFoo.h"
#include "moduleBar.h"

#include <iostream>

int
main()
{
	std::cout << "#### Foo\n";

	ObjectFoo f = makeFoo(10);
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';
	std::cout << '\n';

	std::cout << "# Perform wibble-zorbling\n";
	f.wibble();
	f.zorble();
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';
	std::cout << '\n';
	//////////////////////////////////////////////////////////////////////////////////////
	std::cout << "#### Bar\n";

	ObjectBar b = makeBar();
	std::cout << "\tBar's splinx: " << b.mySplinx << '\n';
	std::cout << "\tBar's yibble: " << b.myYibble << '\n';
	std::cout << '\n';

	std::cout << "# Perform splinx-yibbling\n";
	b.splinx();
	b.yibble();
	std::cout << "\tBar's splinx: " << b.mySplinx << '\n';
	std::cout << "\tBar's yibble: " << b.myYibble << '\n';
	return 0;
}
