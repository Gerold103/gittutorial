#include "moduleBar.h"

static std::string_view splinxes[] = {
	"Splinxle",
	"Sploinx",
	"Splinxy",
	"Splintz",
	"Splinxer",
	"Splainx",
};

void
ObjectBar::splinx()
{
	mySplinx = splinxes[rand() % (sizeof(splinxes) / sizeof(splinxes[0]))];
}

std::string
ObjectBar::toString(uint8_t tabs) const
{
	std::string tab;
	for (uint8_t i = 0; i < tabs; ++i)
		tab += '\t';

	std::string res;
	res += tab + "splinx: " + mySplinx + '\n';
	res += tab + "yibble: " + std::to_string(myYibble) + '\n';
	res += FlibberCollection::toString(tabs);
	return res;
}

ObjectBar
makeBar()
{
	return ObjectBar();
}
