#ifndef YGO_CARDSELECTOR_H
#define YGO_CARDSELECTOR_H

#include <string>
#include <vector>

#include "CardData.h"
#include "DB.h"

namespace ygo
{

class CardSelector
{
public:
    typedef std::vector<std::string> CardList;
    CardSelector();

    StaticCardData query(const std::string& name);

    CardList execute();

    CardSelector& name(const std::string& like);
    CardSelector& cardType(CardType ct);
    CardSelector& attribute(Attribute a);
    CardSelector& monsterType(MonsterType mt);
    CardSelector& type(Type t);
    CardSelector& monsterAbility(MonsterAbility ma);
    CardSelector& level(int l, Operator op);
    CardSelector& attack(int a, Operator op);
    CardSelector& defense(int d, Operator op);
    CardSelector& lpendulum(int d, Operator op);
    CardSelector& rpendulum(int d, Operator op);
    CardSelector& spellType(SpellType st);
    CardSelector& trapType(TrapType tt);

private:
    std::vector<std::string> mQuery;

    DB mDB;
};

}

#endif
