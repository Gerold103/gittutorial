#pragma once

#include "core.h"

struct ObjectBar : public FlibberCollection
{
	void splinx();
	void yibble() { myYibble = (myYibble + 1) * 2; }
	std::string toString(uint8_t tabs) const;

	std::string mySplinx = "Emptio";
	uint64_t myYibble = 0;
};

ObjectBar makeBar();
