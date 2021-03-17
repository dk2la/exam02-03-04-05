#include "ATarget.hpp"

void    ATarget::getHitBySpell(const ASpell& as) const {
	std::cout << this->_type << " has been " << as.getEffects() << std::endl;
}