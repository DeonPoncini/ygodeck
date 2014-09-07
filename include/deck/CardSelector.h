#ifndef YGO_CARDSELECTOR_H
#define YGO_CARDSELECTOR_H

#include <string>
#include <vector>

#include <db/SQLite3.h>
#include <data/CardData.h>

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
    CardSelector& level(int l, db::Operator op);
    CardSelector& attack(int a, db::Operator op);
    CardSelector& defense(int d, db::Operator op);
    CardSelector& lpendulum(int d, db::Operator op);
    CardSelector& rpendulum(int d, db::Operator op);
    CardSelector& spellType(SpellType st);
    CardSelector& trapType(TrapType tt);

private:
    std::vector<std::string> mQuery;
};

}

#endif
