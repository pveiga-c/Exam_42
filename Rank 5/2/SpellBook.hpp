#ifndef SPELLBOOK_HPP
#define SPELLBOOK_HPP


#include <iostream>
#include <string>
#include <map>
#include "ASpell.hpp"

class SpellBook
{
	public:
		SpellBook();
		~SpellBook();

		void learnSpell(ASpell *spell);
		void forgetSpell(const std::string &spell);
};

#endif