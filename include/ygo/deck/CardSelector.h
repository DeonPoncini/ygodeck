#ifndef YGO_DECK_CARDSELECTOR_H
#define YGO_DECK_CARDSELECTOR_H

#include <string>
#include <vector>

#include <mindbw/SQLite3.h>
#include <ygo/data/CardData.h>

namespace ygo
{
namespace deck
{

class CardSelector
{
public:
    typedef std::vector<std::string> CardList;
    CardSelector();

    data::StaticCardData query(const std::string& name);

    CardList execute();

    CardSelector& name(const std::string& like);
    CardSelector& cardType(data::CardType ct);
    CardSelector& attribute(data::Attribute a);
    CardSelector& monsterType(data::MonsterType mt);
    CardSelector& type(data::Type t);
    CardSelector& level(int l, mindbw::Operator op);
    CardSelector& attack(int a, mindbw::Operator op);
    CardSelector& defense(int d, mindbw::Operator op);
    CardSelector& lpendulum(int d, mindbw::Operator op);
    CardSelector& rpendulum(int d, mindbw::Operator op);
    CardSelector& spellType(data::SpellType st);
    CardSelector& trapType(data::TrapType tt);

private:
    std::vector<std::string> mQuery;
};

}
}

#endif
