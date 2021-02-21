#pragma once

#include "ASpell.hpp"
#include <vector>

class SpellBook {
public:
	SpellBook() {}
	~SpellBook() {}

	void        learnSpell(ASpell* at) {
		std::vector<ASpell *>::iterator it_begin = this->_arr.begin();
		std::vector<ASpell *>::iterator it_end = this->_arr.end();
		if (at) {
			while (it_begin != it_end) {
				if ((*it_begin)->getName() == at->getName())
					return ;
				++it_begin;
			}
			this->_arr.push_back(at->clone());
		}
	}

	void        forgetSpell(const std::string& name) {
		std::vector<ASpell *>::iterator it_begin = this->_arr.begin();
		std::vector<ASpell *>::iterator it_end = this->_arr.end();
		while (it_begin != it_end) {
			if ((*it_begin)->getName() == name) {
				delete *it_begin;
				this->_arr.erase(it_begin);
				return ;
			}
			++it_begin;
		}
	}

	ASpell*    createSpell(const std::string& name) {
		std::vector<ASpell *>::iterator it_begin = this->_arr.begin();
		std::vector<ASpell *>::iterator it_end = this->_arr.end();
		while (it_begin != it_end) {
			if ((*it_begin)->getName() == name)
				return (*it_begin);
			++it_begin;
		}
		return (NULL);
	}
private:
	std::vector<ASpell *> _arr;
};