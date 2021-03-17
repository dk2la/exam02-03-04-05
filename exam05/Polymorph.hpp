#pragma once

#include "ASpell.hpp"

class ASpell;

class Polymorph: public ASpell {
public:
	Polymorph(): ASpell("Polymorph", "turned into critter") {}
	~Polymorph() {}

	virtual ASpell* clone(void) const { return new Polymorph(); }
};