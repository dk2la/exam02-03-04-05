#pragma once

#include "ASpell.hpp"

class ASpell;

class Fireball: public ASpell {
public:
	Fireball(): ASpell("Fireball", "burnt to a crisp") {}
	~Fireball() {}

	virtual ASpell* clone(void) const { return new Fireball(); }
};