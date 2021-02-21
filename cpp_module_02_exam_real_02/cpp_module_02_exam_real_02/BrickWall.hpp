#pragma once

#include "ATarget.hpp"

class BrickWall: public ATarget {
public:
	BrickWall(): ATarget("Inconspicuous Red-brick Wall") {}
	~BrickWall() {}

	virtual ATarget* clone(void) const {
		return new BrickWall();
	}
};