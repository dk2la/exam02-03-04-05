#pragma once

#include "ATarget.hpp"
#include <vector>

class TargetGenerator {
public:
	TargetGenerator() {}
	~TargetGenerator() {}

	void        learnTargetType(ATarget* at) {
		std::vector<ATarget *>::iterator it_begin = this->_arr.begin();
		std::vector<ATarget *>::iterator it_end = this->_arr.end();
		if (at) {
			while (it_begin != it_end) {
				if ((*it_begin)->getType() == at->getType())
					return ;
				++it_begin;
			}
			this->_arr.push_back(at->clone());
		}
	}

	void        forgetTargetType(const std::string& name) {
		std::vector<ATarget *>::iterator it_begin = this->_arr.begin();
		std::vector<ATarget *>::iterator it_end = this->_arr.end();
		while (it_begin != it_end) {
			if ((*it_begin)->getType() == name) {
				delete *it_begin;
				this->_arr.erase(it_begin);
				return ;
			}
			++it_begin;
		}
	}

	ATarget*    createTarget(const std::string& name) {
		std::vector<ATarget *>::iterator it_begin = this->_arr.begin();
		std::vector<ATarget *>::iterator it_end = this->_arr.end();
		while (it_begin != it_end) {
			if ((*it_begin)->getType() == name)
				return (*it_begin);
			++it_begin;
		}
		return (NULL);
	}
private:
	std::vector<ATarget *> _arr;
};