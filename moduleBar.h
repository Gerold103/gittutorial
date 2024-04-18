#pragma once

#include "core.h"

#include <vector>

struct ObjectBar
{
	void splinx();
	void yibble() { myYibble = myYibble + 1; }
	void flibber();

	std::string mySplinx = "Emptio";
	uint64_t myYibble = 0;
	std::vector<Flibber> myFlibbers;
};

ObjectBar makeBar();
