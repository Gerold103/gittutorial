#pragma once

#include <string>

struct ObjectBar
{
	void splinx();
	void yibble() { myYibble = (myYibble + 1) * 2; }

	std::string mySplinx = "Emptio";
	uint64_t myYibble = 0;
};

ObjectBar makeBar();
