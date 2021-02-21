//
// Created by Shonna Jakku on 12/23/20.
//

#ifndef EXAM05_ASPELL_HPP
#define EXAM05_ASPELL_HPP


class ASpell {
public:
	ASpell();
	ASpell(const ASpell &);
	ASpell &operator=(const ASpell &);
	std::string const& getName(void) const;
	std::string const& getEffect(void) const;
	void    launch(void) const;
	virtual ASpell*     clone(void) const= 0;
	virtual ~ASpell();
private:
	std::string _name;
	std::string _title;
};


#endif //EXAM05_ASPELL_HPP
