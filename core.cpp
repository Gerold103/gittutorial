#include "core.h"

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

Flibber
makeFlibber()
{
	Flibber f;
	f.myKey = keys[rand() % (sizeof(keys) / sizeof(keys[0]))];
	f.myValue = values[rand() % (sizeof(values) / sizeof(values[0]))];
	return f;
}
