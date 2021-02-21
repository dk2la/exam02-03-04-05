#pragma once

#include "ATarget.hpp"

class LittleKid: public ATarget {
public:
	LittleKid(): ATarget("Target Practice Dummy") {}
	~LittleKid() {}

	virtual ATarget* clone(void) const {
		return new LittleKid();
	}
};