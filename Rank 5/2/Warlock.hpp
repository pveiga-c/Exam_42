#ifndef WARLOCK_HPP
#define WARLOCK_HPP

#include <iostream>
#include <string>
#include <map>
#include "ASpell.hpp"
#include "ATarget.hpp"


class Warlock
{
	private:
		std::string _name;
		std::string _title;
		std::map<std::string, ASpell *> spells;

	public:
		Warlock(const std::string &name, const std::string title);
		~Warlock();

		const std::string& getName() const;
		const std::string& getTitle() const;

		void setTitle(const std::string &title);

		void introduce() const;

		void learnSpell(ASpell *spell);
		void forgetSpell(const std::string spell);
		void launchSpell(std::string spell, const ATarget &target);




};

#endif