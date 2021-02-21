//
// Created by Shonna Jakku on 12/23/20.
//

#ifndef EXAM05_WARLOCK_HPP
#define EXAM05_WARLOCK_HPP

# include <iostream>

class Warlock {
public:
	std::string const& getName(void) const;
	std::string const& getTitle(void) const;
	Warlock(std::string const& name, std::string const& title);
	void        setTitle(std::string const& title);
	void        introduce(void) const;
	~Warlock();
private:
	std::string _name;
	std::string _title;
	Warlock();
	Warlock(const Warlock &);
	Warlock &operator=(const Warlock &);

};


#endif //EXAM05_WARLOCK_HPP
