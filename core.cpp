#include "core.h"

#include <iostream>

static std::string_view keys[] = {
	"Kez",
	"Kex",
	"Kye",
	"Koy",
	"Keem",
	"Kyez",
	"Keh",
	"Kiy",
	"Kae",
	"Kwey",
};

static std::string_view values[] = {
	"Vylue",
	"Vaulue",
	"Vellue",
	"Vyluxe",
	"Velluxe",
	"Vylise",
	"Vellise",
	"Vylume",
	"Vellume",
	"Vyluxe",
};

void
FlibberCollection::flibber()
{
	myFlibbers.push_back(makeFlibber());
}

std::string
FlibberCollection::toString(uint8_t tabs) const
{
	std::cout << "----------------------------- toString1\n";
	if (myFlibbers.size() == 0)
		return "<empty>\n";

	std::cout << "----------------------------- toString2\n";
	std::string tab;
	for (uint8_t i = 0; i < tabs; ++i)
		tab += '\t';

	std::cout << "----------------------------- toString3\n";
	std::string res = tab + "flibbers: <PLACEHOLDER>\n";

	std::cout << "----------------------------- toString4\n";
	return res;
}

Flibber
makeFlibber()
{
	Flibber f;
	f.myKey = keys[rand() % (sizeof(keys) / sizeof(keys[0]))];
	f.myValue = values[rand() % (sizeof(values) / sizeof(values[0]))];
	return f;
}
