#include "moduleFoo.h"
#include "moduleBar.h"

#include <iostream>

int
main()
{
	std::cout << "#### Foo\n";

	ObjectFoo f = makeFoo(10);
	std::cout << f.toString(1) << '\n';

	std::cout << "# Perform wibble-zorbling\n";
	f.wibble();
	f.zorble();
	for (int i = 0; i < 3; ++i)
		f.flibber();
	std::cout << f.toString(1) << '\n';

	std::cout << "# Perform wibble-zorble-flibbering\n";
	f.unwibble();
	f.unwibble();
	f.zorble();
	f.flibber();
	std::cout << f.toString(1);

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
