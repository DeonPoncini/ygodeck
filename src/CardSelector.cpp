#include "CardSelector.h"

#include "Common.h"

#include <mindbw/SQLite3.h>
#include <ygo/data/Serialize.h>

namespace ygo
{
namespace deck
{

CardSelector::CardSelector()
{
}

data::StaticCardData CardSelector::query(const std::string& name)
{
    data::StaticCardData s;
    mindbw::SQLite3 db(DBPATH);
    db.select(mindbw::All(),"card",mindbw::Equal("name",name),
            [&](mindbw::DataMap data) {
                s.name = name.c_str();
                s.cardType = data::toCardType(data["cardType"]);
                s.attribute = data::toAttribute(data["attribute"]);
                s.monsterType = data::toMonsterType(data["monsterType"]);
                s.type = data::toType(data["type"]);
                s.monsterAbility = data::toMonsterType(data["monsterAbility"]);
                s.level = std::atoi(data["level"].c_str());
                s.attack = std::atoi(data["attack"].c_str());
                s.defense = std::atoi(data["defense"].c_str());
                s.lpendulum = std::atoi(data["lpendulum"].c_str());
                s.rpendulum = std::atoi(data["rpendulum"].c_str());
                s.spellType = data::toSpellType(data["spellType"]);
                s.trapType = data::toTrapType(data["trapType"]);
            });
    return s;
}

CardSelector::CardList CardSelector::execute()
{
    CardList list;

    mindbw::SQLite3 db(DBPATH);
    db.select("name","card",mindbw::And(mQuery)
            ,[&](mindbw::DataMap data) {
                for (auto&& kv : data) {
                    list.push_back(kv.second);
                }
            });
    mQuery.clear();
    return list;
}

CardSelector& CardSelector::name(const std::string& like)
{
    mQuery.emplace_back(mindbw::Like("name",like));
    return *this;
}

CardSelector& CardSelector::cardType(data::CardType ct)
{
    mQuery.emplace_back(mindbw::Equal("cardType",fromCardType(ct)));
    return *this;
}

CardSelector& CardSelector::attribute(data::Attribute a)
{
    mQuery.emplace_back(mindbw::Equal("attribute",fromAttribute(a)));
    return *this;
}

CardSelector& CardSelector::monsterType(data::MonsterType mt)
{
    mQuery.emplace_back(mindbw::Equal("monsterType",fromMonsterType(mt)));
    return *this;
}

CardSelector& CardSelector::type(data::Type t)
{
    mQuery.emplace_back(mindbw::Equal("type",fromType(t)));
    return *this;
}

CardSelector& CardSelector::level(int l, mindbw::Operator op)
{
    mQuery.emplace_back(mindbw::Compare("level",std::to_string(l), op));
    return *this;
}

CardSelector& CardSelector::attack(int a, mindbw::Operator op)
{
    mQuery.emplace_back(mindbw::Compare("attack",std::to_string(a), op));
    return *this;
}

CardSelector& CardSelector::defense(int d, mindbw::Operator op)
{
    mQuery.emplace_back(mindbw::Compare("defense",std::to_string(d), op));
    return *this;
}

CardSelector& CardSelector::lpendulum(int d, mindbw::Operator op)
{
    mQuery.emplace_back(mindbw::Compare("lpendulum",std::to_string(d), op));
    return *this;
}

CardSelector& CardSelector::rpendulum(int d, mindbw::Operator op)
{
    mQuery.emplace_back(mindbw::Compare("rpendulum",std::to_string(d), op));
    return *this;
}

CardSelector& CardSelector::spellType(data::SpellType st)
{
    mQuery.emplace_back(mindbw::Equal("spellType",fromSpellType(st)));
    return *this;
}

CardSelector& CardSelector::trapType(data::TrapType tt)
{
    mQuery.emplace_back(mindbw::Equal("trapType",fromTrapType(tt)));
    return *this;
}

}
}
