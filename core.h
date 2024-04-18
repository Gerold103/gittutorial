#pragma once

#include <string>
#include <vector>

struct Flibber
{
	std::string myKey;
	std::string myValue;
};

struct FlibberCollection
{
	void flibber();

	std::vector<Flibber> myFlibbers;
};

Flibber makeFlibber();
