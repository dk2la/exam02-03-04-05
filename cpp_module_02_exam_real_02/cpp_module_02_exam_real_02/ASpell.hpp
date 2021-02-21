#pragma once

#include <iostream>
#include "ATarget.hpp"

class ATarget;

class ASpell {
public:
	ASpell() {}
	ASpell(const std::string& name, const std::string& effects): _name(name), _effects(effects) {}
	ASpell(const ASpell& as) { *this = as; }
	ASpell&     operator=(const ASpell& as) {
		this->_name = as.getName();
		this->_effects = as.getEffects();
		return *this;
	}
	virtual ~ASpell() {}

	std::string     getName(void) const { return this->_name; }
	std::string     getEffects(void) const { return this->_effects; }

	virtual ASpell* clone(void) const = 0;

	void            launch(const ATarget& at) const {
		at.getHitBySpell((*this));
	}

private:
	std::string _name;
	std::string _effects;
};