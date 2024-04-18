#include "moduleBar.h"

static std::string_view splinxes[] = {
	"Splinxle",
	"Sploinx",
	"Splinxy",
};

void
ObjectBar::splinx()
{
	mySplinx = splinxes[rand() % (sizeof(splinxes) / sizeof(splinxes[0]))];
}

ObjectBar
makeBar()
{
	return ObjectBar();
}
