#include "moduleFoo.h"
#include "moduleBar.h"

#include <iostream>

int
main()
{
	std::cout << "#### Foo\n";

	ObjectFoo f = makeFoo();
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';
	std::cout << '\n';

	std::cout << "# Perform wibble-zorbling\n";
	f.wibble();
	f.zorble();
	for (int i = 0; i < 3; ++i)
		f.flibber();
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';
	std::cout << "\tFoo's zorble: " << f.myZorble << '\n';
	if (f.myFlibbers.size() > 0)
	{
		std::cout << "\tFoo's flibbers:\n";
		for (const Flibber& fli : f.myFlibbers)
			std::cout << "\t\t" << fli.myKey << ": " << fli.myValue << '\n';
	}
	std::cout << '\n';

	std::cout << "# Unwibble x2\n";
	f.unwibble();
	f.unwibble();
	std::cout << "\tFoo's wibble: " << f.myWibble << '\n';

	std::cout << '\n';
	//////////////////////////////////////////////////////////////////////////////////////
	std::cout << "#### Bar\n";

	ObjectBar b = makeBar();
	std::cout << b.toString(1) << '\n';

	std::cout << "# Perform splinx-yibbling\n";
	b.splinx();
	b.yibble();
	for (int i = 0; i < 5; ++i)
		b.flibber();
	std::cout << b.toString(1);
	return 0;
}
