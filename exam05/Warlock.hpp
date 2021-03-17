//
//  Warlock.hpp
//  sfssdfsfsfds
//
//  Created by Shonna Jakku on 1/9/21.
//  Copyright © 2021 Shonna Jakku. All rights reserved.
//

#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include "ASpell.hpp"
#include "ATarget.hpp"
#include "SpellBook.hpp"

class SpellBook;
class ASpell;
class ATarget;

class Warlock {
public:
	Warlock(const std::string& name, const std::string& title): _name(name), _title(title) {
		std::cout << this->_name << ": This looks like another boring day." << std::endl;
	}
	~Warlock() {
		std::cout << this->_name << ": My job here is done!" << std::endl;
	}

	const std::string&      getName(void) const { return this->_name; }
	const std::string&      getTitle(void) const { return this->_title; }

	void                    setTitle(const std::string& title) { this->_title = title; }

	void                    introduce(void) const {
		std::cout << this->_name << ": I am " << this->_name << ", " << this->_title << "!" << std::endl;
	}

	void                    learnSpell(ASpell* as) {
		this->_book.learnSpell(as);
	}

	void                    forgetSpell(std::string name) {
		this->_book.forgetSpell(name);
	}

	void                    launchSpell(std::string name, const ATarget& at) {
		ASpell* tmp = this->_book.createSpell(name);
		if (tmp)
			tmp->launch(at);
	}
private:
	Warlock();
	Warlock(const Warlock& );
	Warlock&    operator=(const Warlock& );
	std::string _name;
	std::string _title;
	SpellBook _book;
};