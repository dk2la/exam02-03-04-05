//
// Created by Shonna Jakku on 12/23/20.
//

#include "Warlock.hpp"

void    Warlock::introduce() const {
	std::cout << this->_name << ": " << "I am " << this->_name << ", " << this->_title << "!" << std::endl;
}

void    Warlock::setTitle(std::string const& title) {
	this->_title = title;
}

std::string const&    Warlock::getTitle() const {
	return this->_title;
}

std::string const&    Warlock::getName() const {
	return this->_name;
}

Warlock::Warlock(const std::string& name, const std::string& title): _name(name), _title(title) {
	std::cout << this->_name << ": " << "This looks like another boring day." << std::endl;
}

Warlock::~Warlock() {
	std::cout << this->_name << ": " << "My job here is done!" << std::endl;
}