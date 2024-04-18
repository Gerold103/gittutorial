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
	std::string toString(uint8_t tabs) const;

	std::vector<Flibber> myFlibbers;
};

Flibber makeFlibber();
