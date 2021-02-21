#pragma once

#include <iostream>

class ASpell;

class ATarget {
public:
	ATarget() {}
	ATarget(const std::string& type): _type(type) {}
	ATarget(const ATarget& as) { *this = as; }
	ATarget&     operator=(const ATarget& as) {
		this->_type = as.getType();
		return *this;
	}
	virtual ~ATarget() {}

	const std::string&     getType(void) const { return this->_type; }

	virtual ATarget* clone(void) const = 0;

	void    getHitBySpell(const ASpell& as) const;

private:
	std::string _type;
};
#include "ASpell.hpp"